/** @jsx jsx */
import { jsx } from '@emotion/core'

const styles = {
  header: {
    height: '60px',
    backgroundColor: 'rgba(255,255,255,.3)',
    flexShrink: 0,
  }
}

export default () => {
  return (
    <header className="App-header" css={styles.header}>
      <h1>header</h1>
    </header>
  )
}