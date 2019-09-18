import React from 'react';
//import logo from './logo.svg';
import './App.css';
import { ThemeProvider, createTheme, Content, Words, Header, Loading, Footer, Arwes } from 'arwes';
import System from './System';

const App = () => (
  <ThemeProvider theme={createTheme()}>
    <Arwes className="App">
      <Content>
        <Header animate>
          <h1 style={{ margin: 0 }}>DRD</h1>
        </Header>
        <Words animate>Connecting... </Words><Loading animate/>
        <System />
        <Footer animate>
          <h6>Web v0.00 / API v0.00 / Controller v0.00</h6>
        </Footer>
      </Content>
    </Arwes>
  </ThemeProvider>
);

export default App;
