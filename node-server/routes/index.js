var express = require('express');
var app = require('../app');
var http = require('http');

var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
    res.redirect('/login');
});


router.get('/index', function(req, res, next) {
    res.render('index', {
        title: 'Control Panel',
        name: 'Admin'
    });
});


router.get('/profile', function(req, res, next) {
    res.render('profile', {
        title: 'Control Panel',
        name: 'Admin'
    });
});


router.get('/storage', function(req, res, next) {
    res.render('storage', {
        title: 'Control Panel',
        name: 'Admin'
    });
});

router.get('/activities', function(req, res, next) {
    res.render('activities', {
        title: 'Control Panel',
        name: 'Admin'
    });
});

router.get('/campaigns', function(req, res, next) {
    res.render('campaigns', {
        title: 'Control Panel',
        name: 'Admin'
    });
});

router.get('/provisioning', function(req, res, next) {
    res.render('provisioning', {
        title: 'Control Panel',
        name: 'Admin'
    });
});
router.get('/sensors', function(req, res, next) {
    var ipText = app.ip ? "ACTIVE" : "INACTIVE";
    var sensorIp = app.ip;
    var objects = app.obj;
    console.log(objects);
    res.render('sensors', {
        title: 'Control Panel',
        name: 'Admin',
        status: ipText,
        ip: sensorIp,
        obj: objects
    });
});

router.get('/sensor', function(req, res, next) {
    var ipText = app.ip ? "ACTIVE" : "INACTIVE";
    var sensorIp = app.ip;
    var objects = app.obj;
    console.log(objects);
    res.render('sensor', {
        title: 'Control Panel',
        name: 'Admin',
        status: ipText,
        ip: sensorIp,
        obj: objects
    });
});

router.get('/tables', function(req, res, next) {
    res.render('tables', {
        title: 'Control Panel',
        name: 'Admin'
    });
});

router.get('/calendar', function(req, res, next) {
    res.render('calendar', {
        title: 'Control Panel',
        name: 'Admin'
    });
});


router.get('/login', function(req, res, next) {
    res.render('login');
});

router.get('/logout', function(req, res, next) {
  app.ip = undefined;
  app.obj = undefined;
  res.redirect('/login');
});



router.get('/register', function(req, res, next) {
    var ip = req.param('ip');
    app.ip = ip;
    res.send('OK\nECHO: ' + ip);
});

router.get('/objects', function(req, res, next) {
    var n = req.param('n');
    app.obj = n;
    res.send('OK\nECHO: ' + n);
});

router.get('/refresh', function(req, res, next) {
    refreshSensor(function() {
    res.redirect('/sensors');
    });
});

function refreshSensor(callback) {

    return http.get({
        host: app.ip,
        path: '/'
    }, function(response) {
        // Continuously update stream with data
        var body = '';
        response.on('data', function(d) {
            body += d;
        });
        response.on('end', function() {
            console.log(body);
            callback();
        });
    });

};


module.exports = router;
