// Import Firebase SDK
import { initializeApp } from 'https://www.gstatic.com/firebasejs/9.0.2/firebase-app.js';
import { getDatabase, ref, onValue, runTransaction } from 'https://www.gstatic.com/firebasejs/9.0.2/firebase-database.js';

// Firebase konfiguracija
const firebaseConfig = {
    config
};

// Inicijalizacija Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);


const toggleSwitch = document.getElementById("toggleSwitch");
const switchKnob = document.getElementById("switchKnob");
const statusText = document.getElementById("statusText");
const doorImage = document.getElementById("doorImage");

const imageClosed='./images/vrataZatvorena.jpg';
const imageOpened='./images/vrataOtvorena.jpg';


doorImage.src = status === 'OPEN' ? imageOpened : imageClosed;


//Funkcija za proveru statusa vrata
function checkDoorStatus() {
    const doorStatusRef = ref(database, 'doorStatus');
    onValue(doorStatusRef, (snapshot) => {
        const status = snapshot.val();
        if (status === 'OPEN') {
            statusText.textContent = 'Door is Open';
            switchKnob.style.left = '51px';  
            switchKnob.style.transition = '0.5s'; 
            switchKnob.style.backgroundColor='black';
            doorImage.src = imageOpened;
        } else {
            statusText.textContent = 'Door is Closed';
            switchKnob.style.left = '1px'; 
            switchKnob.style.transition = '0.5s';  
            switchKnob.style.backgroundColor='white';
            doorImage.src = imageClosed;
        }
    });
}



function toggleDoor() {
    const doorStatusRef = ref(database, 'doorStatus');
    
    runTransaction(doorStatusRef, (currentStatus) => {
      
        if (currentStatus === 'OPEN') {
            return 'CLOSED';
        } else {
            return 'OPEN';
        }
    }).then(() => {
        console.log("Door status updated successfully.");
    }).catch((error) => {
        console.error("Error updating door status:", error);
    });
}



checkDoorStatus();


toggleSwitch.onclick = () => {
    toggleDoor();
};



