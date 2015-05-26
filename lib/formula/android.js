var ant = require('./ant');
var androidPlatform = require('./android-platform');
var androidNDK = require('./android-ndk');
var python = require('./python');
var child = require('child_process');
var config = require('../config');
var devnull = require('dev-null');
var mkdirp = require('mkdirp');
var os = require('os');
var path = require('path');


var android = module.exports = {};
var sysName = os.type().toLowerCase();
var hostName = sysName.replace('darwin', 'mac');
var sysArch = os.arch() === 'x64' ? 'x86_64' : 'x86';
var falkorPath = path.join(__dirname, '..', '..', 'deps', 'falkor');
var bastianPath = path.join(falkorPath, 'deps', 'bastian');
var gypScript = path.join(bastianPath, 'tools', 'gyp_bastian');
var gypFile = path.join(falkorPath, 'falkor.gyp');


android.name = 'android';
android.label = 'Falkor Android Libraries';
android.dependencies = [ant, python, androidPlatform, androidNDK];


android.validate = function (done) {
  done({message: 'no rule for validation'});
};

android.install = function (done) {
  config.getAll(function (err, cfg) {

    var args = [
      '-Dbastian_project=' + cfg.falkor.base,
      '-Dbastian_engine=v8',
      '-Dtarget_arch=arm',
      '-Dandroid_target_platform=' + cfg.android.platform,
      '-Darm_version=7',
      '-DOS=android',
      '-Dhost_os=' + hostName,
      gypFile
    ];
    var ndkPrebuiltPath = path.join(
      cfg.android.ndk,
      'toolchains',
      'arm-linux-androideabi-4.8',
      'prebuilt',
      sysName + '-' + sysArch
    );
    var env = {
      CC: path.join(ndkPrebuiltPath, 'bin', 'arm-linux-androideabi-gcc'),
      CXX: path.join(ndkPrebuiltPath, 'bin', 'arm-linux-androideabi-g++'),
      LINK: path.join(ndkPrebuiltPath, 'bin', 'arm-linux-androideabi-g++'),
      AR: path.join(ndkPrebuiltPath, 'bin', 'arm-linux-androideabi-ar'),
      RANLIB: path.join(ndkPrebuiltPath, 'bin', 'arm-linux-androideabi-ranlib'),
      TOOLCHAIN: ndkPrebuiltPath,
      ANDROID_HOME: cfg.android.sdk,
      ANDROID_NDK_ROOT: cfg.android.ndk,
    };

    mkdirp(cfg.falkor.base, function () {
      var gypCall = child.spawn(gypScript, args, {cwd: cfg.falkor.base, env: env});

      gypCall.stdout.on('end', function () {
        var make = child.spawn('make', [], {cwd: path.join(cfg.falkor.base, 'out', 'v8-android_arm'), env: env});
        
        make.stdout.on('end', done);
        make.stdout.pipe(devnull());
        make.stderr.pipe(process.stderr);
      });

      gypCall.stdout.pipe(devnull());
    });
  
  });
};