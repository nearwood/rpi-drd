import React, { useState } from 'react';
import { Project, Appear, Button } from 'arwes';
import useApi from 'use-api';

export default () => {
  const [{ data, isLoading, isError }, doFetch] = useApi(
    null,
    {}
  );
  const [show, setShow] = useState(false);
  return (
    <Project animate header="System Controls">
      <Button style={{float: 'left'}} animate layer='primary' onClick={() => setShow(!show)}>{show ? 'Hide' : 'Show'}</Button>
      <Appear animate show={show}>
        <h6>Web v0.00 / API v{isLoading ? '...' : data.api} / Controller v{data.controller}</h6>
        <Button animate layer='alert' onClick={() => doFetch('http://drd:4482/version')}>Shutdown</Button>
      </Appear>
    </Project>
  );
}
