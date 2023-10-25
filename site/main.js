const addButton = document.getElementById('add-button');
const startButton = document.getElementById('start-button');
const addForm = document.getElementById('add-form');
const startForm = document.getElementById('start-form');


const toggleHiddenElement = (domElement) => {
	if (domElement.style.display === 'none') {
		domElement.style.display = 'block';
	} else {
		domElement.style.display = 'none';
	}
}

addButton.addEventListener('click', () => {
	toggleHiddenElement(addForm);
	toggleHiddenElement(startButton);
	toggleHiddenElement(addButton);
});

startButton.addEventListener('click', () => {
	toggleHiddenElement(addButton);
	toggleHiddenElement(startButton);
	toggleHiddenElement(startForm);
});

addForm.addEventListener("submit", function (event) {
	// Empêcher la soumission automatique du formulaire
	event.preventDefault();

	// Récupérer les valeurs des champs
	var nom = document.getElementById("addFr").value;
	window.alert(nom);
	toggleHiddenElement(addForm);
	toggleHiddenElement(startButton);
	toggleHiddenElement(addButton);

});

startForm.addEventListener("submit", function (event) {
	// Empêcher la soumission automatique du formulaire
	event.preventDefault();

	// Récupérer les valeurs des champs
	var nom = document.getElementById("startFr").value;
	window.alert(nom);
	toggleHiddenElement(startButton);
	toggleHiddenElement(startForm);
	toggleHiddenElement(addButton);

});
