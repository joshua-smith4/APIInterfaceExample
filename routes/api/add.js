var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/:num1/:num2', function(req, res, next) {
  var r = parseInt(req.params.num1) + parseInt(req.params.num2);
  res.json({ result: r });
});

module.exports = router;
