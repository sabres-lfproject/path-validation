var express = require('express');
var router = express.Router();
const shell = require('shelljs');

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index');
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

router.post('/upload', (req, res) => {
  let sampleFile;
  let uploadPath;

  if (!req.files || Object.keys(req.files).length === 0) {
    return res.status(400).send('No files were uploaded.');
  }

  // The name of the input field (i.e. "sampleFile") is used to retrieve the uploaded file
  sampleFile = req.files.sampleFile;
  uploadPath = __dirname + '/../data/' + sampleFile.name;

  // Use the mv() method to place the file somewhere on your server
  sampleFile.mv(uploadPath, function(err) {
    if (err)
      return res.status(500).send(err);

    return res.send('File uploaded!');
  });
});

module.exports = router;
