var axios = require('axios');
var FormData = require('form-data');
var fs = require('fs');
const shell = require('shelljs');
const sabres_port = 3000;

const sendData = async (req, res) => {
  let url = 'http://' + req.body.url + ':' + sabres_port + '/upload';
  let dataPath = __dirname + '/../data/' + req.body.filename;
  try {
    const form = new FormData();
    // const stream = await readFileFromPath('test.txt');
    var stream = fs.createReadStream(dataPath);
    form.append('sampleFile', stream);
    const formHeaders = form.getHeaders();
    let routePath = req.route.path;
    if (routePath == "/sendContent" || routePath == "/upload" || routePath == "/backward") {
      form.append('sendContent', 'true');
      form.append('filename', req.body.filename);
      shell.exec('./xorhash.sh ' + '1.secret' + ' ' + 'new.secret');
    }
    if (routePath == "/chainIdeal") {
      form.append('chainIdeal', 'true');

      form.append('filename', req.body.filename);
      shell.exec('./time.sh');
    }
    if (routePath == "/NIZK") {
      form.append('NIZK', 'true');
     
      form.append('filename', req.body.filename);
      shell.exec('./NIZK.sh');
    }
    var remoteRes = await axios.post(url, form, {
      headers: {
        ...formHeaders,
      },
    });
    // console.log(remoteRes.data);
    return res.json({
      status: "success",
      data: "send data successfully"
    });

  } catch (error) {
    console.log(error);
    return res.status(500).json({
      status: "error",
      error: error.message
    });
  }
}

module.exports = {
  sendData
}