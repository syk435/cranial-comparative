var container;

var camera, scene, renderer;

var mouseX = 0, mouseY = 0;

var windowHalfX = window.innerWidth / 2;
var windowHalfY = window.innerHeight / 2;

var colorMap;
var numberOfColors;

function init() {

	container = document.createElement( 'div' );
	document.body.appendChild( container );

	camera = new THREE.PerspectiveCamera( 45, window.innerWidth / window.innerHeight, 1, 2000 );
	camera.name = "camera";
	camera.position.z = 250;

	// scene

	scene = new THREE.Scene();

	var ambient = new THREE.AmbientLight( 0x101030 );
	ambient.name = "ambientLight";
	scene.add( ambient );

	var directionalLight = new THREE.DirectionalLight( 0xffeedd );
	directionalLight.position.set( 0, 0, 1 );
	directionalLight.name = "directionalLight";
	scene.add( directionalLight );

	//legend settings
	colorMap = 'rainbow';
	numberOfColors = 512;

	//load model in uncolored mode
	loadModel("default");

	renderer = new THREE.WebGLRenderer();
	renderer.setPixelRatio( window.devicePixelRatio );
	renderer.setSize( window.innerWidth, window.innerHeight );
	container.appendChild( renderer.domElement );

	document.addEventListener( 'mousemove', onDocumentMouseMove, false );
	window.addEventListener( "keydown", onKeyDown, true);
	window.addEventListener( 'resize', onWindowResize, false );

}

function loadModel(mode) {
				
	var manager = new THREE.LoadingManager();
	manager.onProgress = function ( item, loaded, total ) {

		console.log( item, loaded, total );

	};

	var onProgress = function ( xhr ) {
		if ( xhr.lengthComputable ) {
			var percentComplete = xhr.loaded / xhr.total * 100;
			console.log( Math.round(percentComplete, 2) + '% downloaded' );
		}
	};

	var onError = function ( xhr ) {
	};

	// texture
	/*var texture = new THREE.Texture();
	var loader = new THREE.ImageLoader( manager );
	loader.load( 'textures/UV_Grid_Sm.jpg', function ( image ) {

		texture.image = image;
		texture.needsUpdate = true;

	} );
	*/

	if(mode=="default"){

		// model
		var loader = new THREE.OBJLoader( manager );
		loader.load( 'assets/patients/head.obj', function ( object ) {

			object.traverse( function ( child ) {

				if ( child instanceof THREE.Mesh ) {

					//child.material.map = texture;
					child.material.color.setHex(0xFFFFFF);
					console.log(child);

				}

			} );

			object.position.y = - 95;
			scene.add( object );

		}, onProgress, onError );

	} else if(mode=="surfaceAreaComparision") {

		// calculate coloring
		calcSurfaceAreaChange(null);
		
		// render model
		var loader = new THREE.OBJLoader( manager );
		loader.load( 'assets/patients/head.obj', function ( object ) {

			object.traverse( function ( child ) {

				if ( child instanceof THREE.Mesh ) {

					//child.material.map = texture;
					child.material.color.setHex(0xFFFFFF);
					//console.log(child);

				}

			} );

			var mesh = object.children[0];
			var geometry = new THREE.Geometry().fromBufferGeometry( mesh.geometry );

			for ( var i = 0; i < geometry.faces.length; i ++ ) {

    			var face = geometry.faces[ i ];
    			face.color.setHex( Math.random() * 0xffffff );

			}

			var mesh1 = new THREE.Mesh( geometry, new THREE.MeshBasicMaterial( { vertexColors: THREE.FaceColors } ));
			mesh1.position.y = - 95;
			scene.add( mesh1 );

		}, onProgress, onError );

		// render legend
		lut = new THREE.Lut( colorMap, numberOfColors );
		lut.setMax( 2000 );
		lut.setMin( 0 );
		legend = lut.setLegendOn();
		legend.scale.set(20,20,20);
		legend.position.x = -50;
		scene.add ( legend );
		var labels = lut.setLegendLabels( { r: 0, g: 0, b: 255, a: 0.8 },{ 'title': 'Pressure', 'um': 'Pa', 'ticks': 5 } );
		scene.add ( labels['title'] );
	
	}

}

function cleanScene () {

	var elementsInTheScene = scene.children.length;
	for ( var i = elementsInTheScene-1; i > 0; i-- ) {
		if ( scene.children [ i ].name != 'camera' &&
			 scene.children [ i ].name != 'ambientLight' &&
			 scene.children [ i ].name != 'directionalLight') {
			scene.remove ( scene.children [ i ] );
		}
	}

}

function onKeyDown(e) {

	if(e.keyCode == 65){
		console.log("A");
		cleanScene();
		loadModel("surfaceAreaComparision");
	}

}

function onWindowResize() {

	windowHalfX = window.innerWidth / 2;
	windowHalfY = window.innerHeight / 2;

	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();

	renderer.setSize( window.innerWidth, window.innerHeight );

}

function onDocumentMouseMove( event ) {

	mouseX = ( event.clientX - windowHalfX ) / 2;
	mouseY = ( event.clientY - windowHalfY ) / 2;

}

			//

function animate() {

	requestAnimationFrame( animate );
	render();

}

function render() {

	camera.position.x += ( mouseX - camera.position.x ) * .05;
	camera.position.y += ( - mouseY - camera.position.y ) * .05;

	camera.lookAt( scene.position );

	renderer.render( scene, camera );

}