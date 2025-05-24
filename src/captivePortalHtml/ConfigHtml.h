#pragma once
constexpr char configHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Contrib Config</title>
    <style>
      input {
        font-family: monospace;
        flex: 1;
        padding: 0.8rem;
        border: none;
        border-radius: 4px;
        font-size: 0.85rem;
        background-color: #3e3e4e;
        color: #fff;
        margin-top: 5px;
      }

      input:focus {
        outline: none;
        box-shadow: 0 0 3px 2px #368db2;
      }

      input::placeholder {
        color: #bbb;
      }

      label {
        font-family: monospace;
        text-transform: uppercase;
        font-size: 0.6rem;
        font-style: normal;
        color: #bbb;
        margin-top: 25px;
      }
    </style>
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
      <pre
        style="
          font-size: 0.4rem;
          text-align: center;
          color: #fff;
          margin-top: 3px;
          margin-bottom: 5px;
          line-height: 0.4rem;
          white-space: pre-line;
        "
      >

&nbsp;█████╗░░█████╗░███╗░░██╗███████╗██╗░██████╗&nbsp;
██╔══██╗██╔══██╗████╗░██║██╔════╝██║██╔════╝&nbsp;
██║░░╚═╝██║░░██║██╔██╗██║█████╗░░██║██║░░██╗&nbsp;
██║░░██╗██║░░██║██║╚████║██╔══╝░░██║██║░░╚██╗
╚█████╔╝╚█████╔╝██║░╚███║██║░░░░░██║╚██████╔╝
&nbsp;╚════╝░░╚════╝░╚═╝░░╚══╝╚═╝░░░░░╚═╝░╚═════╝&nbsp;
      </pre>
      <form action="/submit" style="display: flex; flex-direction: column">
        <!-- Github Username -->
        <label for="username">Github Username</label>
        <div style="display: flex; align-items: center">
          <span
            style="
              color: #d9d9d9;
              font-weight: bold;
              position: absolute;
              margin-left: 11px;
              margin-top: 6px;
            "
            >/</span
          >
          <input
            style="padding-left: 25px"
            type="text"
            value="{{username}}"
            name="username"
            placeholder="HarryHighPants"
            required
          />
        </div>

        <!-- WiFi Credentials -->
        <label for="wifi-ssid">Wifi SSID (2.4Ghz only, case sensitive)</label>
        <input
          type="text"
          value="{{wifi-ssid}}"
          name="wifi-ssid"
          placeholder="WiFi SSID"
          required
        />
        <label for="wifi-password">Wifi Password</label>
        <input
          type="password"
          value="{{wifi-password}}"
          name="wifi-password"
          placeholder="Password"
          required
        />

        <!-- Sync Interval -->
        <label for="interval">Sync interval (Hours)</label>
        <input
          type="number"
          value="{{interval}}"
          name="interval"
          placeholder="4"
          value="4"
          required
        />

        <!-- Dark Mode -->
        <div style="margin-top: 15px">
          <label for="dark-mode">Dark Mode</label>
          <input type="checkbox" name="dark-mode" {{dark-mode}} />
        </div>

        <!-- API URL -->
        <label for="interval">API url override (optional)</label>
        <input
          type="url"
          value="{{url}}"
          name="url"
          placeholder="https://contributions-api.harryab.com/"
        />

        <button
          type="submit"
          style="
            width: 100%;
            padding: 0.8rem;
            margin-top: 25px;
            font-size: 1rem;
            border: none;
            border-radius: 4px;
            background-color: #4caf50;
            color: white;
            cursor: pointer;
            transition: background-color 0.3s;
          "
          onmouseover="this.style.backgroundColor='#45a045'"
          onmouseout="this.style.backgroundColor='#4caf50'"
        >
          Save Configuration
        </button>
      </form>
    </div>
  </body>
</html>
)rawliteral";
