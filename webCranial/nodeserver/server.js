var express = require('express');
var formidable = require('formidable');
var fs = require('fs');
var path = require('path');
var spawn = require('child_process').spawn;

var meshCalc = require('./meshcalculations.js');


var app = express();

app.set('port', (process.env.PORT || 5000));

app.use(express.static(path.join(__dirname, 'public')));

app.get('/596', function(req, res){
    res.sendfile(path.join(__dirname, 'views/index.html'));
});

app.post('/upload', function(req, res){

    // create an incoming form object
    var form = new formidable.IncomingForm();

    // specify that we want to allow the user to upload multiple files in a single request
    form.multiples = true;

    // store all uploads in the /uploads directory
    form.uploadDir = path.join(__dirname, '/public/uploads');

    // every time a file has been uploaded successfully,
    // rename it to it's orignal name
    form.on('file', function(field, file) {
        fs.rename(file.path, path.join(form.uploadDir, file.name));
    });

    // log any errors that occur
    form.on('error', function(err) {
        console.log('An error has occured: \n' + err);
    });

    // once all the files have been uploaded, send a response to the client
    form.on('end', function() {
        res.end('success');
    });

    // parse the incoming request containing the form data
    form.parse(req);

});

//app.get('/curvature', meshCalc.sendCurvature);
//need to move all this into callback for upload so it's not server-wide
app.get('/curvature', function(req, res) {
	var py = spawn('python', ['../pythonserver/CurvatureCalc/curvature.py','public\\\\uploads\\\\'+req.query.file]);
    py.stdout.on('data', function(data){
        //console.log('quack');
    });
    py.stdout.on('end', function(){
       console.log('done running curvature calculation');
        var curveJson;
        fs.readFile('./public/uploads/'+req.query.file+'curvature.json', 'utf8', function (err, data) {
            if (err) throw err;
            curveJson = JSON.parse(data);
            res.json(curveJson);
        });
    });
});

app.get('/curvatureFaces', function(req, res) {
	var py = spawn('python', ['../pythonserver/CurvatureCalc/curvatureFaces.py','public\\\\uploads\\\\'+req.query.file]);
    py.stdout.on('data', function(data){
       /// if(tempi<1){
       ///faceJson += data.toString();}
       ///tempi+=1;
    });
    py.stdout.on('end', function(){
        console.log('sent curvature faces');
        var faceJson='';
        fs.readFile('./public/uploads/'+req.query.file+'faces.json', 'utf8', function (err, data) {
            if (err) throw err;
            faceJson = JSON.parse(data);
            res.json(faceJson);
        });
        ///res.json(faceJson);
    });
});

app.get('/patient/:id', function(req, res) {
    res.send({id:req.params.id, name: "The Name", description: "description"});
});

app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});