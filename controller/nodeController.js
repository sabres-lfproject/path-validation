var axios = require('axios');
var FormData = require('form-data');
var fs = require('fs');
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
    if (routePath == "/sendContent" || routePath == "/upload") {
      form.append('sendContent', 'true');
      form.append('filename', req.body.filename);
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