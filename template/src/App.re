let _ = [%bs.raw {|require('./App.css')|}];
let logo: string = [%bs.raw {|require('./logo.svg')|}];

[@react.component]
let make = () => {
  <div className="App">
    <header className="App-header">
      <img src=logo className="App-logo" alt="logo" />
      <p>
        "Edit "->React.string
        <code> "src/App.re"->React.string </code>
        " and save to reload."->React.string
      </p>
      <div className="App-button-row">
        <a
          className="App-link"
          href="https://reasonml.github.io/"
          target="_blank"
          rel="noopener noreferrer">
          "Learn ReasonML"->React.string
        </a>
        <a
          className="App-link"
          href="https://reasonml.github.io/reason-react/"
          target="_blank"
          rel="noopener noreferrer">
          "Learn ReasonReact"->React.string
        </a>
      </div>
    </header>
  </div>;
};