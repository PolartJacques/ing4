/** @jsx jsx */
import { jsx } from '@emotion/core'
import Channel from '../Channel/Channel';
import Channels from '../Channels/Channels';

const styles = {
  main: {
    backgroundColor: '#373B44',
    flex: '1 1 auto',
    display: 'flex',
    flexDirection: 'row',
    overflow: 'hidden',
  }
}

export default ({
  channel
}) => {
  return (
    <main className="App-main" css={styles.main}>
      <Channels />
      <Channel channel={channel}/>
    </main>
  )
}