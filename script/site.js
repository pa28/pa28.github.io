function load_page(uri) {
    window.open(uri);
}

function close_glossary() {
    var elements = document.getElementsByClassName("gloss_item");

    for (var i = 0; i < elements.length; i++) {
        elements[i].style.display = "none";
    }
}

function click_glossary(id) {
    var x = document.getElementById(id);
    if (x.style.display === "none" || x.style.display === "") {
        close_glossary();
        x.style.display = "block";
    } else {
        x.style.display = "none";
    }
}