function getRandomNumber(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function generateData() {
  let temperature = getRandomNumber(0, 120);
  let pressure = getRandomNumber(900, 1100);
  let humidity = getRandomNumber(40, 60);

  var temperatureValue = document.getElementById('temperature-value');
  var pressureValue = document.getElementById('pressure-value');
  var humidityValue = document.getElementById('humidity-value');

  temperatureValue.textContent =  temperature;
  pressureValue.textContent = pressure;
  humidityValue.textContent =  humidity;
}

generateData();

setInterval(generateData, 3000);
