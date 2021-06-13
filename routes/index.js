var express = require('express');
var router = express.Router();
const shell = require('shelljs');
var FormData = require('form-data');
var fs = require('fs');
var axios = require('axios');

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index');
});

router.get('/dataList', async (req, res) => {
  fs.readdir(__dirname + '/../data/', (err, files) => {
    if (err) {
      return res.status(500).json({
        status: "error",
        error: err
      });
    }
    return res.json({
      status: "success",
      data: files      
    });
  });
});

router.post('/initialize', (req, res) => {
  let number = req.body.number;
  try {
    shell.exec('./initialize.sh ' + number);

    res.json({
      status: "success",
      data: "initialize successful"
    });
  } catch (error) {
    return res.json({
      status: "error",
      error: error
    });
  }
});

router.post('/genproof', (req, res) => {
  let number = req.body.number;
  try {
    shell.exec('./genproof.sh ' + number);

    res.json({
      status: "success",
      data: "generate proof successful"
    });
  } catch (error) {
    return res.json({
      status: "error",
      error: error
    });
  }  
});

router.post('/verifyproof', (req, res) => {
  let number = req.body.number;
  try {
    shell.exec('./verifyproof.sh ' + number, (code, stdout, stderr) => {
      let output = stdout.substring(0, stdout.length - 1);
      if (stdout == 'verify successfully\n') {
        return res.json({
          status: "success",
          data: output
        });
      } else {
        return res.json({
          status: "error",
          error: output
        });
      }
      // console.log('Exit code:', code);
      // console.log('Program output:', stdout);
      // console.log('Program stderr:', stderr);
    });

    // res.json({
    //   status: "success",
    //   data: "verify proof successful"
    // });
  } catch (error) {
    return res.json({
      status: "error",
      error: error
    });
  }  
});

router.post('/upload', (req, res) => {
  let sampleFile;
  let uploadPath;

  if (!req.files || Object.keys(req.files).length === 0) {
    return res.status(400).json({
      status: "error",
      error: "No files were uploaded."
    });
  }

  // The name of the input field (i.e. "sampleFile") is used to retrieve the uploaded file
  sampleFile = req.files.sampleFile;
  uploadPath = __dirname + '/../data/' + sampleFile.name;

  // Use the mv() method to place the file somewhere on your server
  sampleFile.mv(uploadPath, function(err) {
    if (err)
      return res.status(500).json({
        status: "error",
        error: err
      });

    return res.json({
      status: "success",
      data: "File uploaded!"
    });
  });
});

const readFileFromPath = async (dataPath) => {
  return new Promise((resolve, reject) => {
    var stream = fs.createReadStream(dataPath);
    stream.on('data', (chunk) => {
      // console.log(chunk);
    });
    stream.on('end', () => { 
      console.log("hello");
      resolve(stream);
    });
    stream.on('error', err => reject(err));
  });  
};

router.post('/sendData', async (req, res) => {
  let url = 'http://' + req.body.url + '/upload';
  let dataPath = __dirname + '/../data/' + req.body.filename;
  try {
    const form = new FormData();
    // const stream = await readFileFromPath('test.txt');
    var stream = fs.createReadStream(dataPath);
    form.append('sampleFile', stream);
    const formHeaders = form.getHeaders();
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
});

module.exports = router;
