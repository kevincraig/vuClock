let colorpicker;
let slider;
let baseUrl;

window.addEventListener("load", startup, false);

function startup() {
    console.log("Starting Up!");
    colorpicker = document.querySelector("#colorpicker");
    colorpicker.addEventListener("change", updateLedColor, false);
    colorpicker.select();

    slider = document.querySelector('#brightnessslider');
    slider.addEventListener("change", setBrightness, false);
    slider.select();

    baseUrl = window.location.href;
}

function updateLedColor(event) {
    console.log("Event Value: " + event.target.value);
    const color = event.target.value
    const r = parseInt(color.substr(1, 2), 16)
    const g = parseInt(color.substr(3, 2), 16)
    const b = parseInt(color.substr(5, 2), 16)
    setColor(r, g, b);
}

function getTime() {

    if (baseUrl == null) {
        baseUrl = window.location.href;
    }

    jQuery.ajax({
        url: baseUrl + 'api/time',
        type: 'GET',
    })
        .done(function (data, textStatus, jqXHR) {
            console.log('HTTP Request Succeeded: ' + jqXHR.status)
            console.log(data)
            document.getElementById('time').innerHTML = data.time.current_time
        })
        .fail(function (jqXHR, textStatus, errorThrown) {
            console.log('HTTP Request Failed')
            document.getElementById('time').innerHTML = 'Error'
        })
        .always(function () {
            /* ... */
        })
}

function setColor(r, g, b) {
    if (baseUrl == null) {
        baseUrl = window.location.href;
    }
    console.log(r, g, b);

    jQuery.ajax({
        url: baseUrl + 'api/ledcolor',
        type: 'POST',
        data: {
            r: r,
            g: g,
            b: b

        }
    })
}

function setBrightness(brightness) {
    if (baseUrl == null) {
        baseUrl = window.location.href;
    }


    jQuery.ajax({
        url: baseUrl + 'api/brightness',
        type: 'POST',
        data: {
            level: brightness.target.value
        }
    })
}

function getConfig() {
    if (baseUrl == null) {
        baseUrl = window.location.href;
    }

    jQuery.ajax({
        url: baseUrl + 'api/config',
        type: 'GET',
    })
        .done(function (data, textStatus, jqXHR) {
            console.log('HTTP Request Succeeded: ' + jqXHR.status)
            console.log(data)
            document.getElementById('ssid').innerHTML = data.config.ssid;
            document.getElementById('ntp_server').innerHTML = data.config.ntp_server;

        })
        .fail(function (jqXHR, textStatus, errorThrown) {
            console.log('HTTP Request Failed')
            document.getElementById('ssid').innerHTML = 'Error'
            document.getElementById('ntp_server').innerHTML = 'Error'
        })

}

//Run on page load
getTime()
getConfig()
//Run every minute
setInterval(getTime, 60000)