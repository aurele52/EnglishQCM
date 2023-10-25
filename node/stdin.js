const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

exports.question = function(q , cb ){

    var response;

    rl.setPrompt(q);
    rl.prompt();

    rl.on('line', (userInput) => {
        response = userInput;
        rl.close();
    });

    rl.on('close', () => {
        return cb(response);
    });
};
var age  = arki.question("how old are you? ", resp => {
    console.log(resp);
});
