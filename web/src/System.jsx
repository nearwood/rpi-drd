import React, { useState } from 'react';
import { Project, Appear, Button, Words } from 'arwes';
import useApi from 'use-api';

export default (props) => {
  const [{ data, isLoading, isError }, api] = useApi();

  const [disableControls, setDisabled] = useState(props.disabled);
  const [show, setShow] = useState(false);

  const onShutdown = () => {
    setDisabled(true);
    api('http://drd:4482/shutdown');
  };

  return (
    <Project animate header="System Controls">
      <Button style={{float: 'left'}} animate layer='primary' onClick={() => setShow(!show)}>{show ? 'Hide' : 'Show'}</Button>
      <Appear animate show={show}>
        <Button animate layer='alert' disabled={disableControls} onClick={onShutdown}>Shutdown</Button>
        {isLoading && <Words animate>Shutting down... :(</Words>}
        {isError === true && <Words animate layer='alert'>Error Shutting down!</Words>}
      </Appear>
    </Project>
  );
}
