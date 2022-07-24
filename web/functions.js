texts = []

function init_function() 
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "apps_state.json", false ); // false for synchronous request
    xmlHttp.send( null );
    response = xmlHttp.responseText
    json = JSON.parse(response);
    var divtext = document.getElementById("divtext");
    divtext.innerHTML = "";

    for (var i = 0; i < json.apps.length; i++) {
        // create div
        let index = i
        var div = document.createElement("div");

        // create label for text
        var text = document.createElement("label");
        texts.push(text);
        div.appendChild(text);

        let stop_btn = document.createElement("button");
        stop_btn.innerHTML = "Stop";
        stop_btn.onclick = function () {
            stop(index);
        };
        div.appendChild(stop_btn);

        let start_btn = document.createElement("button");
        start_btn.innerHTML = "Start";
        start_btn.onclick = function () {
            start(index);
        };
        div.appendChild(start_btn);

        divtext.appendChild(div);
    }
}

function state_update_loop() 
{
    setInterval(function(){
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.open( "GET", "apps_state.json", true ); // false for synchronous request
        xmlHttp.onload = function (e) {
            response = xmlHttp.responseText
            json = JSON.parse(response);

            for (var i = 0; i < json.apps.length; i++) {
                text = texts[i];
                text.innerHTML = json.apps[i].name + ": " + json.apps[i].state;
            }
        };
        xmlHttp.send( null );
    }, 100);
}

function stop_all() 
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "stop_all.action", false ); // false for synchronous request
    xmlHttp.send( null );
}

function start_all() 
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "start_all.action", false ); // false for synchronous request
    xmlHttp.send( null );
}

function stop(i) 
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "stop.action?index=" + i, false ); // false for synchronous request
    xmlHttp.send( null );
} 

function start(i) 
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "start.action?index=" + i, false ); // false for synchronous request
    xmlHttp.send( null );
} 