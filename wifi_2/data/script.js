document.addEventListener("DOMContentLoaded", function () {
    const turnButtons = document.querySelectorAll(".turn-btn");
    turnButtons.forEach(button => {
        const moveCommand = button.getAttribute("data-move");
        button.addEventListener("mousedown", function () {
            moveCar(moveCommand);
        });
        button.addEventListener("mouseup", function () {
            moveCar('s');
        });
        button.addEventListener("mouseleave", function () {
            moveCar('s');
        });
        button.addEventListener("touchstart", function () {
            moveCar(moveCommand);
        });
        button.addEventListener("touchend", function () {
            moveCar('s');
        });
        button.addEventListener("touchcancel", function () {
            moveCar('s');
        });
    });
});

function moveCar(move) {
    fetch('/Car?move=' + move)
        .then(response => response.text())
        .then(data => {
            console.log('Car response:', data);
        })
        .catch(error => console.error('Error moving car:', error));
}

function moveArm(arm, angle) {
    fetch('/Arm?arm=' + arm + '&angle=' + angle)
        .then(response => response.text())
        .then(data => {
            console.log('Arm response:', data);
        })
        .catch(error => console.error('Error moving arm:', error));
}

function toggleBuzz() {
    fetch('/Buzz')
        .then(response => response.text())
        .then(data => {
            console.log('Buzz response:', data);
        })
        .catch(error => console.error('Error toggling buzz:', error));
}

function toggleTrack() {
    fetch('/Track')
        .then(response => response.text())
        .then(data => {
            console.log('Track response:', data);
        })
        .catch(error => console.error('Error toggling track:', error));
}

function toggleAvoidance() {
    fetch('/Avoidance')
        .then(response => response.text())
        .then(data => {
            console.log('Avoidance response:', data);
        })
        .catch(error => console.error('Error toggling avoidance:', error));
}

function toggleFollow() {
    fetch('/Follow')
        .then(response => response.text())
        .then(data => {
            console.log('Follow response:', data);
        })
        .catch(error => console.error('Error toggling follow:', error));
}

function toggleLed(button) {
    if (button.innerText === "LED Off") {
        button.innerText = "LED On";
        button.style.backgroundColor = "green";
    } else {
        button.innerText = "LED Off";
        button.style.backgroundColor = "red";
    }
    fetch('/Led')
        .then(response => response.text())
        .then(data => {
            console.log('Led response:', data);
        })
        .catch(error => console.error('Error fetching led:', error));
}

function toggleClaw(button) {
    if (button.innerText === "Claw Open") {
        button.innerText = "Claw Close";
        button.style.backgroundColor = "green";
    } else {
        button.innerText = "Claw Open";
        button.style.backgroundColor = "red";
    }
    fetch('/Claw')
        .then(response => response.text())
        .then(data => {
            console.log('Claw response:', data);
        })
        .catch(error => console.error('Error fetching claw:', error));
}

function handleMusic(track) {
    fetch('/Music?track=' + track)
        .then(response => response.text())
        .then(data => {
            console.log(`Music response for ${track}:`, data);
        })
        .catch(error => console.error(`Error playing music track ${track}:`, error));

}

function toggleReset() {
    fetch('/Reset')
        .then(() => {
            document.getElementById('slider1').value = 100;
            document.getElementById('slider2').value = 55;
            document.getElementById('slider3').value = 150;
            updateSliderValue(1, 100);
            updateSliderValue(2, 55);
            updateSliderValue(3, 150);
            document.getElementById('music-options').value = "0";
            const ledButton = document.querySelector('button[onclick="toggleLed(this)"]');
            ledButton.innerText = "LED Off";
            ledButton.style.backgroundColor = "red";
            const clawButton = document.querySelector('button[onclick="toggleClaw(this)"]');
            clawButton.innerText = "Claw Close";
            clawButton.style.backgroundColor = "green";

        })
        .catch(error => console.error('Error resetting:', error));
}

function updateDistance() {
    fetch('/Distance')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.text();
        })
        .then(data => {
            document.getElementById('distance').innerText = data;
        })
        .catch(error => {
            console.error('Error fetching distance:', error);
            document.getElementById('distance').innerText = "NA";
        });
}

function updateSliderValue(sliderNumber, value) {
    document.getElementById(`slider${sliderNumber}-value`).innerText = value;
    if (sliderNumber === 1) {
        moveArm('chassis', value);
    }
    else if (sliderNumber === 2) {
        moveArm('shoulder', value);
    }
    else if (sliderNumber === 3) {
        moveArm('elbow', value);
    }
}

var joystick1 = nipplejs.create({
    zone: document.getElementById('joystick-zone-1'),
    mode: 'static',
    position: { left: '50%', top: '50%' },
    color: 'blue',
    size: 300
});

let lastDirection = '';

joystick1.on('dir', function (evt, data) {
    if (!data || !data.direction) return;

    let direction = data.direction.angle;
    console.log('Joystick direction:', direction);
    if (direction !== lastDirection) {
        lastDirection = direction;
        let moveCommand = 's';

        switch (direction) {
            case 'up':
                moveCommand = 'f';
                break;
            case 'down':
                moveCommand = 'b';
                break;
            case 'left':
                moveCommand = 'l';
                break;
            case 'right':
                moveCommand = 'r';
                break;
        }
        console.log('Moving: ' + moveCommand);
        moveCar(moveCommand);
    }
});

joystick1.on('end', function () {
    moveCar('s');
    lastDirection = '';
    console.log('Joystick stopped');
});

setInterval(updateDistance, 500);