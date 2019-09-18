import React, { useState, useEffect } from 'react';
//import logo from './logo.svg';
import './App.css';
import { ThemeProvider, createTheme, Content, Words, Header, Loading, Footer, Arwes } from 'arwes';
import System from './System';


const App = () => {
  
  return (
    <ThemeProvider theme={createTheme()}>
      <Arwes className="App">
        <Content>
          <Header animate>
            <h1 style={{ margin: 0 }}>DRD</h1>
          </Header>
          <Words animate>Connecting...</Words><Loading animate/>
          <System />
          <Footer animate>
          </Footer>
        </Content>
      </Arwes>
    </ThemeProvider>
  );
};

export default App;
