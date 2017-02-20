function colorMapping(n) {
	//0 to 240, where 240 is blue
    return 'hsl(' + n + ',100%,50%)';
}

function calcSurfaceAreaChange(geometry){
	//
}

//c'mon bro, callbacks pls
function runCurvatureAnalysis(vertexCurvatures, faces) {
	//TEST: render model for MEAN curvature
	//find maxCurv, use vertexCurvatures[4] array and faces to recreate patient geometry,
	//select vertex color with Math.abs(vertexCurvatures[3])/maxMean * colorScale
	var MEAN = 3;
	var MIN = 0;
	var MAX = 1;
	var GAUSS = 2;

	var maxMean = 0;
	for (var key in vertexCurvatures) {
  		if (vertexCurvatures.hasOwnProperty(key)) {	
    		 if (Math.abs(vertexCurvatures[key][MAX]) > maxMean) {maxMean = Math.abs(vertexCurvatures[key][MAX]);}
  		}
	}
	console.log('maxMean -> ' + maxMean);

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
  			fc.vertexColors[0] = new THREE.Color(colorMapping((1-(Math.abs(vertexCurvatures[faces[key][0]][MAX])/maxMean))*240));
  			fc.vertexColors[1] = new THREE.Color(colorMapping((1-(Math.abs(vertexCurvatures[faces[key][1]][MAX])/maxMean))*240));
  			fc.vertexColors[2] = new THREE.Color(colorMapping((1-(Math.abs(vertexCurvatures[faces[key][2]][MAX])/maxMean))*240));
  			geometry.faces.push(fc);
  		}
  	}
  	geometry.computeBoundingSphere();
  	//geometry.computeVertexNormals();
  	//geometry.computeFaceNormals();
  	var material = new THREE.MeshBasicMaterial({ vertexColors: THREE.VertexColors });
  	var mesh = new THREE.Mesh(geometry, material);
  	console.log(mesh);
  	scene.add(mesh);
}
