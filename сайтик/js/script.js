function getRandomNumber(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }

  function generateData() {
    let temperature = getRandomNumber(0, 120);
    let pressure = getRandomNumber(900, 1100);
    let humidity = getRandomNumber(40, 60);

    document.getElementById('weather-info').innerText = `Temperature: ${temperature}°C, Pressure: ${pressure} hPa, Humidity: ${humidity}%`;
  }

  generateData();

  setInterval(generateData, 3000);
