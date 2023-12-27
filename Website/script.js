var blood = 2;
var theme = {}

var count = 0;

function generateRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

function loadImage(index){
    var label = ['I', 'c', 'first', 'ok', 'palm', 'palm_moved', 'thumb'];
    return label[index];
}

function _post_topic(){
    var xhr = new XMLHttpRequest();
    var url = "http://140.123.91.93:8000/post_topic";
    var data = {"topic": theme[0].toString()+","+theme[1].toString()+","+theme[2].toString(),"count":count.toString()};
    
    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "application/json");
    
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            // Request was successful, handle the response
            console.log("Response from server:", xhr.responseText);
        } else if (xhr.readyState == 4) {
            // Request completed but with an error
            console.error("Error:", xhr.status, xhr.statusText);
        }
    };
    
    xhr.send(JSON.stringify(data));    
}

function _post_count(){
    var xhr = new XMLHttpRequest();
    var url = "http://140.123.91.93:8000/post_count";
    var data = {"count":count.toString()};
    
    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "application/json");
    
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            // Request was successful, handle the response
            console.log("Response from server:", xhr.responseText);
        } else if (xhr.readyState == 4) {
            // Request completed but with an error
            console.error("Error:", xhr.status, xhr.statusText);
        }
    };
    
    xhr.send(JSON.stringify(data));    
}

function changeImage(index) {
    // Get the image element by its ID
    var image = document.getElementById("gesture_image");

    // Change the src attribute
    image.src = "./images/"+loadImage(theme[index])+".png";
}

document.addEventListener('DOMContentLoaded', function() {
    theme[0] = generateRandomInt(0,6);
    theme[1] = generateRandomInt(1,6);
    theme[2] = generateRandomInt(0,6);

    count = 0
    console.log('Theme : '+theme[0]+theme[1]+theme[2]);
    console.log("Initial : "+"./images/"+loadImage(theme[count])+".png");
    changeImage(count);

    _post_topic();
});

function showBlood(){
    var message = document.getElementById("monster_blood");
    var text = "Blood : "+ blood
    message.innerText = text;
}

function makeRequest() {
    var xhr = new XMLHttpRequest();
    xhr.withCredentials = true;

    xhr.addEventListener("readystatechange", function() {
    if(this.readyState === 4) {
        var data = JSON.parse(this.responseText);
        console.log(parseInt(data.label, 10));
        _post_count();
        if(parseInt(data.label, 10)>-1){
            if(parseInt(data.label, 10)==theme[count]){
                blood = blood -1;
                if(blood<=0){
                    var image = document.getElementById("monster_image");
                    if(theme[count]==0)
                        image.src = "./images/kiwawa.png";
                    else image.src = "./images/monster_die.jpg";
                    alert("Defeat the moster!");
                    // location.reload();
                }else
                    alert("Attack sucess!");
            }else alert("Attack failed!");
    
            count++;
            _post_count();

            if(count>2 && blood > 0){
                count =-1;
                _post_count();
                alert("You lose");
                location.reload();
            }else{
                changeImage(count);
            }
        
        }
    }});

    xhr.open("GET", "http://140.123.91.93:8000/get_label");
    xhr.send();
}

// Make the initial request
makeRequest();

// Set up an interval to make requests every 1 second
setInterval(makeRequest, 2000);
setInterval(showBlood, 500);