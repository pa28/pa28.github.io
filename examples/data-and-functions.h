struct PanelStatusLights {
    ...
};

struct chassis {
    ...
    sim::VirtualPanel <PanelStatusLights> status_lights;

    void status_lights_writer(sim::VirtualPanel<PanelStatusLights>::data_ptr_t &lights);

    void status_lights_reader(sim::VirtualPanel<PanelStatusLights>::data_ptr_t const &lights);

    ...
};
