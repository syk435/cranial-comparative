function colorMapping(value,maximum) {
	minimum = 0.0;
	ratio = 2.0 * (value-minimum) / (maximum - minimum)
	b = Math.trunc(Math.max(0, 255*(1 - ratio)))
    r = Math.trunc(Math.max(0, 255*(ratio - 1)))
    g = 255 - b - r
    return 'rgb(' + r + ',' + g + ','+b + ')';
}

function calcSurfaceAreaChange(geometry){
	//
}

//c'mon bro, callbacks pls
function runCurvatureAnalysis(vertexCurvatures, faces) {
	//TEST: render model for MEAN curvature
	//find maxCurv, use vertexCurvatures[4] array and faces to recreate patient geometry,
	//select vertex color with Math.abs(vertexCurvatures[3])/maxCurv * colorScale
	//console.log(typeof(vertexCurvatures));
	//console.log(faces);
	//vertexCurvatures =  JSON.parse(vertexCurvatures);
	//faces = JSON.parse(faces.trim());
	console.log(typeof(faces));
	var MEAN = 3;
	var MIN = 0;
	var MAX = 1;
	var GAUSS = 2;

	var maxCurv = 0;
	var prevMax = 0;
	var prevPrev = 0;
	var prev3 = 0;
	for (var key in vertexCurvatures) {
  		if (vertexCurvatures.hasOwnProperty(key)) {	
    		 if (Math.abs(vertexCurvatures[key][MEAN]) > maxCurv) {prev3 = prevPrev; prevPrev = prevMax; prevMax = maxCurv;maxCurv = Math.abs(vertexCurvatures[key][MEAN]);}
  		}
	}
    var IS_JSON = true;
    try
    {
        var json = $.parseJSON(vertexCurvatures);
    }
    catch(err)
    {
        IS_JSON = false;
    }
    //console.log(vertexCurvatures);
    console.log('IS_JSON: ' + IS_JSON);
    console.log('maxCurv -> ' + maxCurv);
	console.log('prevMax -> ' + prevMax);
	console.log('prevPrev -> ' + prevPrev);
	console.log('prev3 ->' + prev3);
	if(maxCurv>=(3*prevMax)) {maxCurv=prevMax;}
	if(maxCurv>=(3*prevPrev)) {maxCurv=prevPrev;}
	console.log('maxCurv -> ' + maxCurv);
	///****TEST*****
	var testCIEL = .1
	maxCurv = testCIEL;
	//run std deviation, clip out huge outliers later ***

	//Later load geometry via JSONLoader..?
	var geometry = new THREE.Geometry();
	for (var key in vertexCurvatures) {
  		if (vertexCurvatures.hasOwnProperty(key)) {	
  			geometry.vertices.push(new THREE.Vector3(vertexCurvatures[key][4][0],vertexCurvatures[key][4][1],vertexCurvatures[key][4][2]));
  		}
  	}
  	for (var key in faces) {
  		if (faces.hasOwnProperty(key)) {
  			var fc = new THREE.Face3(faces[key][0]-1,faces[key][1]-1,faces[key][2]-1);
  			var curv0 = Math.abs(vertexCurvatures[faces[key][0]][MEAN]);
  			var curv1 = Math.abs(vertexCurvatures[faces[key][1]][MEAN]);
  			var curv2 = Math.abs(vertexCurvatures[faces[key][2]][MEAN]);
  			if(curv0>testCIEL){curv0=testCIEL;}
  			if(curv1>testCIEL){curv1=testCIEL;}
  			if(curv2>testCIEL){curv2=testCIEL;}
  			fc.vertexColors[0] = new THREE.Color(colorMapping(curv0,maxCurv));
  			fc.vertexColors[1] = new THREE.Color(colorMapping(curv1,maxCurv));
  			fc.vertexColors[2] = new THREE.Color(colorMapping(curv2,maxCurv));
  			geometry.faces.push(fc);
  		}
  	}
  	geometry.computeBoundingSphere();
  	//geometry.computeVertexNormals();
  	//geometry.computeFaceNormals();
  	var material = new THREE.MeshBasicMaterial({ vertexColors: THREE.VertexColors });
  	var mesh = new THREE.Mesh(geometry, material);
  	//console.log(mesh);
  	scene.add(mesh);
}
