import React, { useState } from 'react';
import { Project, Appear, Button } from 'arwes';

export default () => {
  const [show, setShow] = useState(false);
  return (
    <Project animate header="System Controls">
      <Button style={{float: 'left'}} animate layer='primary' onClick={() => setShow(!show)}>{show ? 'Hide' : 'Show'}</Button>
      <Appear animate show={show}>
        <Button animate layer='alert'>Shutdown</Button>
      </Appear>
    </Project>
  );
}
