function colorMapping1(value,maximum) {
	minimum = 0.0;
	ratio = 2.0 * (value-minimum) / (maximum - minimum);
	b = Math.trunc(Math.max(0, 255*(1 - ratio)));
  r = Math.trunc(Math.max(0, 255*(ratio - 1)));
  g = 255 - b - r;
  return 'rgb(' + r + ',' + g + ','+b + ')';
}

function colorMapping2(c) {
  r = 0;
  b = 0;
  g = 0;
  if (c<0.5) {
    r = 0;
    b = Math.trunc(Math.max(0, 255*(1-2*c)));
    g = Math.trunc(Math.max(0, 255*(2*c)));
  } else{
    r = Math.trunc(Math.max(0, 255*(2*c-1)));
    b = 0;
    g = Math.trunc(Math.max(0, 255*(2-2*c)));
  }
  return 'rgb(' + r + ',' + g + ','+b + ')';
}

function calcSurfaceAreaChange(geometry){
	//
}

function calcAsymmetry(){
  //
}

function runCurvatureDiff(vertexCurvatures1, vertexCurvatures2, faces, corr) {
  var MEAN = 3;
  var MIN = 0;
  var MAX = 1;
  var GAUSS = 2;

  var sceneIndex = 4;
  var testCIEL = .1
  var Beta = .5

  var geometry = new THREE.Geometry();
  for (var key in vertexCurvatures2) {
    if (vertexCurvatures2.hasOwnProperty(key)) { 
      geometry.vertices.push(new THREE.Vector3(vertexCurvatures2[key][4][0],vertexCurvatures2[key][4][1],vertexCurvatures2[key][4][2]));
    }
  }
  for (var key in faces) {
    if (faces.hasOwnProperty(key)) {
      var fc = new THREE.Face3(faces[key][0]-1,faces[key][1]-1,faces[key][2]-1);
      var curv0 = Math.abs(Math.abs(vertexCurvatures2[faces[key][0]][MEAN]) - Math.abs(vertexCurvatures1[corr[faces[key][0]-1][0]][MEAN]));
      var curv1 = Math.abs(Math.abs(vertexCurvatures2[faces[key][1]][MEAN]) - Math.abs(vertexCurvatures1[corr[faces[key][1]-1][0]][MEAN]));
      var curv2 = Math.abs(Math.abs(vertexCurvatures2[faces[key][2]][MEAN]) - Math.abs(vertexCurvatures1[corr[faces[key][2]-1][0]][MEAN]));
      curv0 = 1-Math.exp(-1*Beta*(curv0/(Math.abs(Math.abs(vertexCurvatures2[faces[key][0]][MEAN]) + Math.abs(vertexCurvatures1[corr[faces[key][0]-1][0]][MEAN])))));
      curv1 = 1-Math.exp(-1*Beta*(curv1/(Math.abs(Math.abs(vertexCurvatures2[faces[key][1]][MEAN]) + Math.abs(vertexCurvatures1[corr[faces[key][1]-1][0]][MEAN])))));
      curv2 = 1-Math.exp(-1*Beta*(curv2/(Math.abs(Math.abs(vertexCurvatures2[faces[key][2]][MEAN]) + Math.abs(vertexCurvatures1[corr[faces[key][2]-1][0]][MEAN])))));
      /*if(curv0>testCIEL){curv0=testCIEL;}
      if(curv1>testCIEL){curv1=testCIEL;}
      if(curv2>testCIEL){curv2=testCIEL;}*/
      fc.vertexColors[0] = new THREE.Color(colorMapping2(curv0));
      fc.vertexColors[1] = new THREE.Color(colorMapping2(curv1));
      fc.vertexColors[2] = new THREE.Color(colorMapping2(curv2));
      geometry.faces.push(fc);
    }
  }

  geometry.computeBoundingSphere();
  var material = new THREE.MeshLambertMaterial({ vertexColors: THREE.VertexColors });
  geometry.computeVertexNormals();
  material.shading = THREE.SmoothShading;
  var mesh = new THREE.Mesh(geometry, material);
  scenes[sceneIndex].add(mesh);
  scenes[sceneIndex].userData.camera.lookAt(mesh.position);
}

//c'mon bro, callbacks pls
function runCurvatureAnalysis(vertexCurvatures, faces, sceneIndex) {
	//TEST: render model for MEAN curvature
	//find maxCurv, use vertexCurvatures[4] array and faces to recreate patient geometry,
	//select vertex color with Math.abs(vertexCurvatures[3])/maxCurv * colorScale
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
  
  /*console.log('maxCurv -> ' + maxCurv);
	console.log('prevMax -> ' + prevMax);
	console.log('prevPrev -> ' + prevPrev);
	console.log('prev3 ->' + prev3);*/
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
  			fc.vertexColors[0] = new THREE.Color(colorMapping1(curv0,maxCurv));
  			fc.vertexColors[1] = new THREE.Color(colorMapping1(curv1,maxCurv));
  			fc.vertexColors[2] = new THREE.Color(colorMapping1(curv2,maxCurv));
  			geometry.faces.push(fc);
  		}
  	}
  	geometry.computeBoundingSphere();
  	var material = new THREE.MeshLambertMaterial({ vertexColors: THREE.VertexColors });
    geometry.computeVertexNormals();
    material.shading = THREE.SmoothShading;
  	var mesh = new THREE.Mesh(geometry, material);
  	//console.log(mesh);
  	scenes[sceneIndex].add(mesh);
    scenes[sceneIndex].userData.camera.lookAt(mesh.position);
}
