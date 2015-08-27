function onFormSubmit() {
  console.log('onSubmit');
  if (!validate()) {
    return false;
  }
  var georeferenced = document.getElementById('input-georeferenced').value;
  var similar = document.getElementById('input-similar').value;
  var url = '/v1/autogeoreference?georeferenced=' + georeferenced + '&similar=' + similar;
  document.getElementById('loading').style.display = 'block';
  ajax(url, processResponse);
  return false;
}

function validate() {
  var valid = true;
  var georeferenced = document.getElementById('input-georeferenced');
  var similar = document.getElementById('input-similar');

  if (georeferenced.value) {
    georeferenced.className = '';
  } else {
    valid = false;
    georeferenced.className = 'error';
  }
  if (similar.value) {
    similar.className = '';
  } else {
    valid = false;
    similar.className = 'error';
  }
  return valid;
}

function processResponse(data) {
  document.getElementById('dialog').style.display = 'none';
  document.getElementById('result').style.display = 'block';
  var georeferencedImg = document.getElementById('img-georeferenced');
  var similarImg = document.getElementById('img-similar');
  georeferencedImg.src = 'data:image/jpg;base64,' + data['georeferenced_image'];
  similarImg.src = 'data:image/jpg;base64,' + data['similar_image'];
  document.getElementById('loading').style.display = 'none';
}

function ajax(url, callback) {
  var xmlhttp = new XMLHttpRequest();

  xmlhttp.onreadystatechange = function() {
      if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
          var data = JSON.parse(xmlhttp.responseText);
          callback(data);
      }
  }
  xmlhttp.open("GET", url, true);
  xmlhttp.send();
}
