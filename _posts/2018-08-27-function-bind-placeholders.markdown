---
layout: post
title:  "std::function - std::bind - std::placeholders"
date:   2018-08-27 14:04:06 -0400
categories: C++ Templates STL
excerpt_separator: <!--more-->
---
### Showing Status &mdash; Getting Commands

One of the things I will want to do is display the simulated PDP8 status on the wonderful
[PiDP8](http://obsolescence.wixsite.com/obsolescence/pidp-8) front pannel, and communicate
front panel switch events to the simulated PDP8.
<!--more-->
I could write
all of this in the program, and in fact I did that in the previous version. But that
would restrict running the software on a Raspberry Pi because of the hardware
requirements. I think it would be nice to be able to run the software on many different
platforms. Maybe have a GUI simulated front panel. Maybe remote the front panel back to
a PiDP8. This requires that reading and writing the data between sources and
destinations be controlled to prevent data races. For this purpose I wrote a simple
template support class [VirtualPanel.h](https://github.com/pa28/PiDP-8-sim-cpp/tree/master/src/sim_r/VirtualPanel.h):

{% highlight cpp linenos %}
#pragma once

#include <memory>
#include <ratio>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <functional>

namespace sim {

    /**
     * @brief A template class to provide locking and notification around virtual
     * front panel input and output.
     * @tparam DataStruct A data structure type that contains values flowing from a
     * source to a destination.
     */
    template<class DataStruct>
    class VirtualPanel {
    public:
        using data_type_t = DataStruct;
        using data_ptr_t = std::unique_ptr<data_type_t>;
        using time_point_t = std::chrono::steady_clock::time_point;

        /**
         * @brief (constructor)
         */
        VirtualPanel() : data{}, data_ready{false},
                         mutex{}, data_condition_lck{},
                         data_condition{} {
            data = std::make_unique<data_type_t>();
            last_data_write = std::chrono::steady_clock::now();
        }

        /**
         * @brief Get the time point of the last data
         * @return sim::VirtualPanel::time_point_t
         * @details The time point may be used to determine if the data is more recent
         * than held by the caller. The read is mitigated by a shared mutex lock to
         * ensure thread safety and prevent race conditions.
         */
        time_point_t get_last_data_write() const {
            std::shared_lock <std::shared_mutex> lock(mutex);
            return last_data_write;
        }

        /**
         * @brief Arrange to read the data.
         * @param reader A function that will copy data from the data structure to the
         * destination.
         * @return sim::VirtualPanel::time_point_t
         * @details A shared lock is obtained and held against the mutex. The reader
         * function is called with a shared pointer reference to the data; this function
         * should copy or use the data but must return smartly since no more data can be
         * received while the mutex is held. The last data write timepoint is returned
         * ensuring the caller has the time associated with the data just read.
         */
        time_point_t read_data(

    std::function<
        void(std::unique_ptr<data_type_t>
        const &)> reader) const {
            std::shared_lock <std::shared_mutex> lock(mutex);
            reader(data);
            return last_data_write;
        }

        /**
         * @brief Arrange to write the data
         * @param writer A function that will copy data from the source to the data
         * structure.
         * @details A unique lock is obtained and held against the mutex. The writer
         * function is called with a shared pointer reference to the data structure;
         * this function should set the structure value and return smartly since no
         * other access to the VirtualPannel is possible while the lock is held.
         *
         * The last data write timepoint is updated and threads waiting at @wait_on_data()
         * are notified.
         */
        void write_data(std::function<void(std::unique_ptr < data_type_t > &)> writer) {
            std::unique_lock <std::shared_mutex> lock(mutex);
            writer(data);
            last_data_write = std::chrono::steady_clock::now();
            notify();
        }

        /**
         * @brief Wait until new data is available
         * @param time_point The last timepoint returned from a call to
         * @get_last_data_write() or @read_data(), or a default initialized time_point_t.
         * @details While the provided time point is the same as, or later than the last
         * data write time point wait on the data condition variable. See @write_data().
         */
        void wait_on_data(time_point_t time_point) {
            std::unique_lock <std::mutex> lock(data_condition_lck);
            while (time_point >= last_data_write) data_condition.wait(lock);
        }

    protected:
        std::unique_ptr <data_type_t> data;

        bool data_ready;

        time_point_t last_data_write;

        mutable std::shared_mutex mutex;
        mutable std::mutex data_condition_lck;
        std::condition_variable data_condition;

        /**
         * @brief Notify threads waiting at @wait_on_data().
         */
        void notify() {
            std::unique_lock <std::mutex> lock(data_condition_lck);
            data_ready = true;
            data_condition.notify_all();
        }
    };

} // namespace sim
{% endhighlight %}

Names are hard. I hope to come up with a better one for this class soon.

The class creates data storage as provided by the template parameter, and holds it in a
unique pointer. There are two methods for interacting with the data: synchronously
through polling using the `get_last_data_write()`
method; and asynchronously by using the `wait_on_data()`
method. Whichever method you choose, once you decide to read data from the structure or
write data to the structure you call `read_data()` or
`write_data()` respectively.

### Readers and Writers &mdash; std::function

`read_data()` takes a single argument `reader`, and `write_data()` takes a single argument
`writer`. Each of these arguments are of the same type, `reader` is `const` and `writer` is
not:

{% highlight cpp %}
std::function<void(std::unique_ptr<data_type_t>const &)> reader

std::function<void(std::unique_ptr < data_type_t > &)> writer
{% endhighlight %}

This syntax tells the compiler `reader` and
`writer` are functions which may be passed a single
argument which is the `std::unique_ptr` to the data. We
can pass in any [Callable](https://en.cppreference.com/w/cpp/named_req/Callable) target.
The trick is to be able to construct the Callable. First we have to specialize
`VirtualPanel` for a particular data structure and
define reader and writer functions. The details of the data and functions are not
important at the moment:

struct PanelStatusLights {
    ...
};

{% highlight cpp %}
struct chassis {
    ...
    sim::VirtualPanel<PanelStatusLights> status_lights;

    void status_lights_writer(
        sim::VirtualPanel<PanelStatusLights>::data_ptr_t &lights);

    void status_lights_reader(
        sim::VirtualPanel<PanelStatusLights>::data_ptr_t const &lights);

    ...
};
{% endhighlight %}

### Callable Objects &mdash; std::bind and std::placeholders

We can pass any &mdash; properly typed callable &mdash; to `reader` and `writer`, I will
show you two forms: lambdas and `std::bind`. First lambdas:

{% highlight cpp linenos %}
...
status_lights.write([this](status_lights_t::data_ptr_t &data_ptr) {
    this->status_lights_writer(data_ptr);
});
...
{% endhighlight %}

That is quite strait forward and easy to understand (if you are familiar with lamdas.
Using `std::bind` is a little more obscure (until you
become familiar with it), especially when passing a member method. We have to provide
the address of the member method and a pointer to the object. We did this manually in
the lambda on line 3. Finally since we want to pass one argument to our Callable we must
provide a place holder for that argument. Many of the examples I found by searching
would bind an actual value instead of a place holder. This is not as useful.

{% highlight cpp linenos %}
...
status_lights.write_data(std::bind(
    &chassis::status_lights_writer,
    this,
    std::placeholders::_1));
...
{% endhighlight %}

As long as `writer` is going to be a method using <code
`std::bind` is a more elegant solution. Of course you
could write the implementation of `status_lights_writer`
in the lambda. It is always good to have choice.
