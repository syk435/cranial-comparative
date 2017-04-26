$('#upload-input').on('change', function(){

    var files = $(this).get(0).files;

    if (files.length > 0){
        // One or more files selected, process the file upload
        // create a FormData object which will be sent as the data payload in the
        // AJAX request
        var formData = new FormData();

        // loop through all the selected files
        for (var i = 0; i < files.length; i++) {
            var file = files[i];

            // add the files to formData object for the data payload
            formData.append('uploads[]', file, file.name);
        }
    }

    $.ajax({
        url: '/upload',
        type: 'POST',
        data: formData,
        processData: false,
        contentType: false,
        success: function(data){
            console.log('upload successful!');
            //on success load uploaded mesh into vis canvas, 'default', assume 1 mesh for now
            var fileName = formData.get('uploads[]').name;
            firstFileName = fileName;
            loadModel(fileName,'default','0');
        }
    });
});

$('#upload-input1').on('change', function(){

    var files = $(this).get(0).files;

    if (files.length > 0){
        // One or more files selected, process the file upload
        // create a FormData object which will be sent as the data payload in the
        // AJAX request
        var formData = new FormData();

        // loop through all the selected files
        for (var i = 0; i < files.length; i++) {
            var file = files[i];

            // add the files to formData object for the data payload
            formData.append('uploads1', file, file.name);
        }
    }

    $.ajax({
        url: '/upload',
        type: 'POST',
        data: formData,
        processData: false,
        contentType: false,
        success: function(data){
            console.log('upload successful!');
            //on success load uploaded mesh into vis canvas, 'default', assume 1 mesh for now
            var fileName = formData.get('uploads1').name;
            secondFileName = fileName;
            loadModel(fileName,'default','2');
        }
    });
});
