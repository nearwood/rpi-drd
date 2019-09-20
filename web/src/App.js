import React, { useState } from 'react';
//import logo from './logo.svg';
import './App.css';
import { ThemeProvider, createTheme, Content, Words, Header, Loading, Footer, Arwes } from 'arwes';
import System from './System';
import useApi from 'use-api';


const App = () => {
  const [{ data, isLoading, isError }] = useApi('http://drd:4482/version', false);

  let connect = <Words animate layer='success'>Connected!</Words>;
  if (isLoading) {
    connect = <><Words animate>Connecting...</Words><Loading animate/></>;
  } else if (isError) {
    connect = <Words layer='alert'>Error connecting 😢</Words>;
  }
  
  return (
    <ThemeProvider theme={createTheme()}>
      <Arwes className="App">
        <Content>
          <Header animate>
            <h1 style={{ margin: 0 }}>DRD</h1>
          </Header>
          {connect} 
          <System disabled={!data}/>
          <Footer animate>
            <h6>Web v0.00 {isLoading || isError ? '' : `API v${data.api} / Controller v${data.controller}`}</h6>
          </Footer>
        </Content>
      </Arwes>
    </ThemeProvider>
  );
};

export default App;
