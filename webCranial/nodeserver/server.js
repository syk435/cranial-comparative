var express = require('express');
var formidable = require('formidable');
var fs = require('fs');
var path = require('path');
var spawn = require('child_process').spawn;

var meshCalc = require('./meshcalculations.js');

users = ['salem','user'];
passwords = ['1234','password'];

var app = express();

app.set('port', (process.env.PORT || 5000));

app.use(express.static(path.join(__dirname, 'public')));

var server = app.listen(app.get('port'), function() {
    console.log('Node app is running on port', app.get('port'));
});

var io = require('socket.io')(server);

io.on('connection', function(client) {
    console.log('Client connected');

    client.on('disconnect', function(data) {
        console.log('Client disconnected');
    });
});

app.get('/', function(req, res){

  var auth = req.get("authorization");
  if (!auth) {
    res.set("WWW-Authenticate", "Basic realm=\"Authorization Required\"");
    return res.status(401).send("Authorization Required");
  } else {
    var credentials = new Buffer(auth.split(" ").pop(), "base64").toString("ascii").split(":");
    if (users.indexOf(credentials[0]) > -1 && credentials[1] === passwords[users.indexOf(credentials[0])]) {
      // The username and password are correct, so the user is authorized.
      console.log(credentials[0] + " logged in");
      res.sendFile(path.join(__dirname, 'views/index.html'));
    } else {
      // The user typed in the username or password wrong.
      return res.status(403).send("Access Denied (incorrect credentials)");
    }
  }

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

app.get('/registerImages', function(req, res) {
    var py = spawn('python', ['../pythonserver/Registration/PatientReg.py','public\\\\uploads\\\\'+req.query.file1,'public\\\\uploads\\\\'+req.query.file2]);
    py.stdout.on('data', function(data){
    });
    py.stdout.on('end', function(){
        console.log('registration complete');
        var corrJson;
        var faceJson;
        var curveJson1;
        var curveJson2;
        fs.readFile('./public/uploads/'+req.query.file2+'Corr.json', 'utf8', function (err, data) {
            if (err) throw err;
            corrJson = JSON.parse(data);

            fs.readFile('./public/uploads/'+req.query.file2+'faces.json', 'utf8', function (err, data) {
                if (err) throw err;
                faceJson = JSON.parse(data);
                
                fs.readFile('./public/uploads/'+req.query.file1+'curvature.json', 'utf8', function (err, data) {
                    if (err) throw err;
                    curveJson1 = JSON.parse(data);
                    
                    fs.readFile('./public/uploads/'+req.query.file2+'curvature.json', 'utf8', function (err, data) {
                        if (err) throw err;
                        curveJson2 = JSON.parse(data);
                        
                        io.emit('reg corr data', { corrJson: corrJson, faceJson: faceJson, curveJson1: curveJson1, curveJson2: curveJson2 });
                    });
                });
            });
        });
    });
    res.status(200).json({ok:true})
});