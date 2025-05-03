const wordleGrid = document.getElementById('wordle-grid');
const keyboard = document.getElementById('keyboard');
const messageContainer = document.getElementById('message-container');
let currentRow = 0;
let currentCol = 0;
let targetWord = '';
let isGameOver = false;
let validWords = [];

// Create the Wordle grid
for (let i = 0; i < 6; i++) {
    for (let j = 0; j < 5; j++) {
        const cell = document.createElement('div');
        wordleGrid.appendChild(cell);
    }
}

// Fetch the words.txt file and select a random word
fetch('words.txt')
    .then(response => response.text())
    .then(data => {
        validWords = data.split('\n').filter(word => word.length === 5);
        resetGame();
    })
    .catch(error => console.error('Error loading words.txt:', error));

// Handle keyboard input
keyboard.addEventListener('click', (event) => {
    if (event.target.classList.contains('key')) {
        const key = event.target.textContent;
        handleKeyPress(key);
    }
});

document.addEventListener('keydown', (event) => {
    const key = event.key.toUpperCase();
    handleKeyPress(key);
});

function handleKeyPress(key) {
    if (key === 'ESCAPE') {
        resetGame();
        return;
    }

    if (isGameOver) {
        return; // Ignore input if the game is over
    }

    if (key === 'ENTER') {
        submitGuess();
    } else if (key === 'BACKSPACE') {
        deleteLetter();
    } else if (key.length === 1 && key.match(/[A-Z]/)) {
        insertLetter(key);
    }
}

function insertLetter(letter) {
    if (currentCol < 5) {
        const cell = wordleGrid.children[currentRow * 5 + currentCol];
        cell.textContent = letter;
        currentCol++;
    }
}

function deleteLetter() {
    if (currentCol > 0) {
        currentCol--;
        const cell = wordleGrid.children[currentRow * 5 + currentCol];
        cell.textContent = '';
    }
}

function submitGuess() {
    if (currentCol === 5) {
        const guess = getCurrentGuess();
        if (guess.length === 5) {
            if (validWords.includes(guess.toLowerCase())) {
                checkGuess(guess);
                currentRow++;
                currentCol = 0;
            } else {
                showMessage('Word not in dictionary.');
            }
        }
    } else {
        showMessage('Please enter a 5-letter word.');
    }
}

function getCurrentGuess() {
    const start = currentRow * 5;
    const end = start + 5;
    const guess = [];
    for (let i = start; i < end; i++) {
        guess.push(wordleGrid.children[i].textContent);
    }
    return guess.join('');
}

function checkGuess(guess) {
    const cells = Array.from(wordleGrid.children).slice(currentRow * 5, currentRow * 5 + 5);
    const targetArray = targetWord.split('');
    const keyboardKeys = document.querySelectorAll('.key');

    cells.forEach((cell, index) => {
        if (cell.textContent === targetArray[index]) {
            cell.style.backgroundColor = 'green';
            targetArray[index] = null;
            updateKeyboardColor(cell.textContent, 'green');
        }
    });

    cells.forEach((cell) => {
        if (cell.style.backgroundColor !== 'green' && targetArray.includes(cell.textContent)) {
            cell.style.backgroundColor = '#cca300';
            targetArray[targetArray.indexOf(cell.textContent)] = null;
            updateKeyboardColor(cell.textContent, '#cca300');
        } else if (cell.style.backgroundColor !== 'green') {
            cell.style.backgroundColor = '#333333';
            updateKeyboardColor(cell.textContent, '#333333');
        }
    });

    if (guess === targetWord) {
        isGameOver = true;
        showMessage('Congratulations! You guessed the word!');
    } else if (currentRow === 5) {
        isGameOver = true;
        showMessage(`Game over! The word was ${targetWord}.`);
    }
}

function updateKeyboardColor(letter, color) {
    const key = Array.from(keyboard.querySelectorAll('.key')).find(key => key.textContent === letter);
    if (key) {
        const currentColor = key.style.backgroundColor;
        if (currentColor === 'green' || currentColor === 'black') {
            return; // Do not change color if it's already green or black
        }
        if (currentColor === 'yellow') {
            if (color === 'gray') {
                return; // Do not change yellow to gray
            }
            if (color === 'green') {
                key.style.backgroundColor = 'green';
                return; // Change to green
            }
            if (currentColor === 'gray') {
                key.style.backgroundColor = 'yellow';
                return; // Change to new color
            }
        }

        key.style.backgroundColor = color;
    }
}

function showMessage(message) {
    messageContainer.textContent = message;
    messageContainer.classList.add('show');
    setTimeout(() => {
        messageContainer.classList.remove('show');
    }, 3000); // Hide the message after 3 seconds
}

function resetGame() {
    currentRow = 0;
    currentCol = 0;
    isGameOver = false;
    targetWord = validWords[Math.floor(Math.random() * validWords.length)].toUpperCase();
    console.log('New target word:', targetWord); // For debugging purposes

    // Clear the Wordle grid
    for (let i = 0; i < 6; i++) {
        for (let j = 0; j < 5; j++) {
            const cell = wordleGrid.children[i * 5 + j];
            cell.textContent = '';
            cell.style.backgroundColor = '';
        }
    }

    // Reset keyboard colors
    const keyboardKeys = document.querySelectorAll('.key');
    keyboardKeys.forEach(key => {
        key.style.backgroundColor = '';
        key.classList.remove('dark-grey', 'dark-yellow');
    });

    // Clear any messages
    messageContainer.textContent = '';
    messageContainer.classList.remove('show');
}
