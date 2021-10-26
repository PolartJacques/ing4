/** @jsx jsx */
import { jsx } from '@emotion/core'
import moment from 'moment'

import {unified} from 'unified'
import remarkParse from 'remark-parse'
import remarkRehype from 'remark-rehype'
import rehypeFormat from 'rehype-format'
import rehypeStringify from 'rehype-stringify'
import './Messages.css'

const md = unified()
  .use(remarkParse)
  .use(remarkRehype)
  .use(rehypeFormat)
  .use(rehypeStringify)

const styles ={
  messages: {
    flex: '1 1 auto',
    height: '100%',
    overflow: 'auto',
    '& ul': {
      'margin': 0,
      'padding': 0,
      'textIndent': 0,
      'listStyleType': 0,
    },
  },
  message: {
    margin: '.2rem',
    padding: '.2rem',
    // backgroundColor: '#66728E',
    ':hover': {
      backgroundColor: 'rgba(255,255,255,.2)',
    },
  }
}

export default ({
  messages
}) => {
  return (
    <div css={styles.messages}>
      <ul>
        { messages.map( (message, i) => (
          <li key={i} css={styles.message}>
            <p>
              <span>{message.author}</span>
              {' '}
              <span>{moment(message.creation).format('H:M - D/MMM/YYYY')}</span>
            </p>
            <div dangerouslySetInnerHTML={{ __html: md.processSync(message.content).value }}></div>
          </li>
        ))}
      </ul>
    </div>
  )
}