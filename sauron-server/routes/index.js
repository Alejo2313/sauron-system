var express = require('express');
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
    res.render('sensors', {
        title: 'Control Panel',
        name: 'Admin'
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

module.exports = router;
