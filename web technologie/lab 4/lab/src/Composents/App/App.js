import './App.css';
/** @jsx jsx */
import { jsx } from '@emotion/core'
import Header from '../Header/Header';
import Footer from '../Footer/Footer';
import Main from '../Main/Main';

const styles = {
  root: {
    boxSizing: 'border-box',
    display: 'flex',
    flexDirection: 'column',
    backgroundColor: '#565E71',
    padding: '50px',
  },
  headerLogIn: {
    backgroundColor: 'red',
  },
  headerLogOut: {
    backgroundColor: 'blue',
  }  
}

export default ({
  channel = {
    name: 'Fake channel'
  }
}) => {  
  return (
    <div className="App" css={styles.root}>
      <Header />
      <Main channel={channel}/>
      <Footer />
    </div>
  );
}
