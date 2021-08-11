var express = require('express');
var router = express.Router();
const shell = require('shelljs');
var FormData = require('form-data');
var fs = require('fs');
var axios = require('axios');
var { sendData } = require("../controller/nodeController");

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index');
});

const getNamesAndNodeNumbers = async () => {
  let hostname = process.env.HOSTNAME;
  // console.log('The value of HOSTNAME is:', hostname);
  var remoteRes = await axios.get('/containers/json', {
    socketPath: '/var/run/docker.sock'
  });
  let data = remoteRes.data;
  let localName = "";
  let nodeNumber = 0;

  for (let idx in data) {
    let name = data[idx].Names[0];
    if (name.substr(0, 19) == "/sabres_sabres_node") {
      nodeNumber++;
    }

    let id = data[idx].Id;
    if (id.substr(0, 12) == hostname) {
      localName = name.substr(1);
    }
  }

  return { localName, nodeNumber };
}

router.get('/info', async (req, res) => {
  let { localName, nodeNumber } = await getNamesAndNodeNumbers();
  res.json({"status": "success", "message": `There are ${nodeNumber} sabres node are running. This node's name is ${localName}`});
});

router.get('/clearData', async (req, res) => {
  try {
    shell.rm('-rf', __dirname + '/../data/*');

    res.json({
      status: "success",
      data: "clear data successfully"
    });
  } catch (error) {
    return res.json({
      status: "error",
      error: error
    });
  }  
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
    shell.exec('./genhashonly2.sh ' + number);
    res.json({
      status: "success",
      data: "initialize successfully"
    });
  } catch (error) {
    return res.json({
      status: "error",
      error: error
    });
  }
});

router.post('/genproof', (req, res) => {
  let first_number = req.body.first_number;
  let second_number = req.body.second_number;
  try {
    // shell.exec('./genproof.sh ' + number);
    console.log(__dirname);
    if (!second_number) {
      console.log("no second number");
      shell.exec('./genproof.sh ' + first_number);
    } else {
      console.log("second number");
      shell.exec('./genproof.sh ' + first_number + " " + second_number);
    }
    
    res.json({
      status: "success",
      data: "generate proof successfully"
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

router.post('/xorhash', (req, res) => {
  let first_secret = req.body.first_secret;
  let second_secret = req.body.second_secret;
  try {
    shell.exec('./xorhash.sh ' + first_secret + ' ' + second_secret);

    res.json({
      status: "success",
      data: "xor hash successfully"
    });
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

// const readFileFromPath = async (dataPath) => {
//   return new Promise((resolve, reject) => {
//     var stream = fs.createReadStream(dataPath);
//     stream.on('data', (chunk) => {
//       // console.log(chunk);
//     });
//     stream.on('end', () => { 
//       console.log("hello");
//       resolve(stream);
//     });
//     stream.on('error', err => reject(err));
//   });  
// };

router.post('/sendData', sendData);

module.exports = router;
