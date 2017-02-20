var express = require('express');
var meshCalc = require('./meshcalculations.js');
var testCurv = require('./public/assets/curvatures/curvature.json');
var testFaces = require('./public/assets/curvatures/faces.json');


var app = express();

app.set('port', (process.env.PORT || 5000));

app.use(express.static('public'));

//app.get('/curvature', meshCalc.sendCurvature);
//temporary hardcode for test
app.get('/curvature', function(req, res) {
	res.json(testCurv);
});

app.get('/curvatureFaces', function(req, res) {
	res.json(testFaces);
});

app.get('/patient/:id', function(req, res) {
    res.send({id:req.params.id, name: "The Name", description: "description"});
});

app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});