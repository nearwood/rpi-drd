import React from 'react';
//import logo from './logo.svg';
import './App.css';
import { ThemeProvider, createTheme, Content, Words, Header, Loading, Footer, Arwes } from 'arwes';
import System from './System';

import useApi from 'use-api';

const App = () => {
  const [{ data, isLoading, isError }, doFetch] = useApi(
    'http://drd:4482/version',
    { version: {} }
  );

  return (
    <ThemeProvider theme={createTheme()}>
      <Arwes className="App">
        <Content>
          <Header animate>
            <h1 style={{ margin: 0 }}>DRD</h1>
          </Header>
          <Words animate>Connecting... </Words><Loading animate/>
          <System />
          <Footer animate>
            <h6 onClick={() => doFetch()}>Web v0.00 / API v{isLoading ? '...' : data.version.api} / Controller v0.00</h6>
          </Footer>
        </Content>
      </Arwes>
    </ThemeProvider>
  );
};

export default App;
