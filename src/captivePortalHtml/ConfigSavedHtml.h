#pragma once
constexpr char configSavedHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Contrib Config Saved</title>
  </head>
  <body
    style="
      font-family: 'Courier New', Courier, monospace;
      background-color: #1e1e2e;
      color: #ffffff;
      margin: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      text-align: center;
    "
  >
    <div
      style="
        background-color: #2e2e3e;
        padding: 2rem;
        margin: 2rem;
        margin-top: 0;
        border-radius: 8px;
        width: 90%;
        max-width: 350px;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
      "
    >
      <h1 style="color: #fff; margin-top: 3px; margin-bottom: 5px">
        Config Saved
      </h1>
      <p>The device will now restart with the new configuration</p>
    </div>
  </body>
</html>
)rawliteral";
