#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define IN1 D1  // Motor IN1
#define IN2 D2  // Motor IN2

const char* ssid = "Train-Controller-AP";
const char* password = "CHANGE_THIS_PASSWORD"; // Set your own WPA2 password (8+ characters)

ESP8266WebServer server(80);

int motorSpeed = 0;
String direction = "stop";

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html><html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>Train Controller</title>
  <script src='https://cdn.jsdelivr.net/npm/@bernii/gauge.js@1.3.7/dist/gauge.min.js'></script>
  <style>
    body { font-family: Arial, sans-serif; background: #2c3e50; color: #fff; text-align: center; padding: 20px; }
    .panel { background: #34495e; padding: 20px; border-radius: 12px; display: inline-block; box-shadow: 0 8px 20px rgba(0,0,0,0.3); }
    h1 { font-size: 2em; }
    .btn { margin: 10px; padding: 15px 30px; font-size: 18px; border: none; border-radius: 10px; cursor: pointer; }
    .fwd { background: #27ae60; }
    .back { background: #2980b9; }
    .stop { background: #c0392b; }
    input[type=range] { width: 80%; }
    canvas { margin-top: 20px; }
  </style>
</head>
<body>
  <h1>Train Controller</h1>
  <div class='panel'>
    <p><strong>Motor Direction</strong></p>
    <button class='btn fwd' onclick='sendCmd("forward")'>Forward</button>
    <button class='btn back' onclick='sendCmd("backward")'>Backward</button>
    <button class='btn stop' onclick='sendCmd("stop")'>STOP</button>
    <hr>
    <p><strong>Throttle</strong></p>
    <input type='range' min='0' max='1023' value='0' id='speedSlider' oninput='updateSpeed(this.value)'>
    <p>Speed: <span id='speedVal'>0</span></p>
    <canvas id='speedGauge' width='300' height='300'></canvas>
  </div>

<script>
var opts = {
  angle: 0.15,
  lineWidth: 0.44,
  radiusScale: 1,
  pointer: {
    length: 0.6,
    strokeWidth: 0.035,
    color: '#000000'
  },
  limitMax: false,
  limitMin: false,
  colorStart: '#6FADCF',
  colorStop: '#8FC0DA',
  strokeColor: '#E0E0E0',
  generateGradient: true,
  highDpiSupport: true,
  staticLabels: {
    font: '10px sans-serif',
    labels: [0, 256, 512, 768, 1023],
    color: '#fff',
    fractionDigits: 0
  },
};

var target = document.getElementById('speedGauge');
var gauge = new Gauge(target).setOptions(opts);
gauge.maxValue = 1023;
gauge.setMinValue(0);
gauge.animationSpeed = 32;
gauge.set(0);

function sendCmd(cmd) {
  fetch('/cmd?dir=' + cmd);
}
function updateSpeed(val) {
  document.getElementById('speedVal').innerText = val;
  fetch('/speed?val=' + val);
}
function fetchSpeed() {
  fetch('/readspeed')
    .then(response => response.text())
    .then(data => {
      const speed = parseInt(data) || 0;
      document.getElementById('speedVal').innerText = speed;
      document.getElementById('speedSlider').value = speed;
      gauge.set(speed);
    });
}
setInterval(fetchSpeed, 1000);
</script>
</body>
</html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void applyMotorControl() {
  if (direction == "forward") {
    analogWrite(IN1, motorSpeed);
    digitalWrite(IN2, LOW);
  } else if (direction == "backward") {
    digitalWrite(IN1, LOW);
    analogWrite(IN2, motorSpeed);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}

void handleDirection() {
  direction = server.arg("dir");
  applyMotorControl();
  server.send(204, "text/plain", "");
}

void handleSpeed() {
  motorSpeed = constrain(server.arg("val").toInt(), 0, 1023);
  applyMotorControl();
  server.send(204, "text/plain", "");
}

void handleReadSpeed() {
  server.send(200, "text/plain", String(motorSpeed));
}

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  analogWriteRange(1023);
  analogWriteFreq(1000);

  WiFi.softAP(ssid, password);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleDirection);
  server.on("/speed", handleSpeed);
  server.on("/readspeed", handleReadSpeed);
  server.begin();
}

void loop() {
  server.handleClient();
}