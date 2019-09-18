import React from 'react';
//import logo from './logo.svg';
import './App.css';
import { ThemeProvider, createTheme, Arwes } from 'arwes';
import { Button } from 'arwes';

const App = () => (
  <ThemeProvider theme={createTheme()}>
    <Arwes>
      <h1>DRD</h1>
      <Button>Shutdown</Button>
    </Arwes>
  </ThemeProvider>
);

export default App;
