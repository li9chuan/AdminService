import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    socket: {
      isConnected: false,
      message: '',
      reconnectError: false,
      isAuthOk: false
    },
    SvrList: [],
    ExecResult: ''
  },
  getters: {
    IsAuthOk(state) { return state.socket.isAuthOk },
  },
  mutations:{
    StateAuth (state, visible)  { state.socket.isAuthOk  = visible },
    ClearExecResult(state) { state.ExecResult = "" },
    SOCKET_ONOPEN (state, event)  {
        Vue.prototype.$socket = event.currentTarget
        state.socket.isConnected = true
        Vue.prototype.$socket.send('{"msgtype":"MsgSvrList"}');
        console.info("SOCKET_ONOPEN!");
    },
    SOCKET_ONCLOSE (state, event)  {
        state.socket.isConnected  = false
        //state.socket.isAuthOk     = false
        console.info("SOCKET_ONCLOSE!");
    },
    SOCKET_ONERROR (state, event)  {
        //console.error(state, event)
        console.info("SOCKET_ONERROR!");
    },
    // default handler called for all methods
    SOCKET_ONMESSAGE (state, message)  {
      let Buffer = require('buffer').Buffer;
      let reader = new FileReader();

      reader.onload = function(e) {
        const zlib = require('zlib');
        const bbuffer = Buffer.from(reader.result, 'binary');

        zlib.inflate(bbuffer, (err, buffer) => {
          if (!err) {
            let b = new Blob([buffer]);
            let frd = new FileReader();

            frd.onload = function(e){
              let json = JSON.parse(e.target.result)
              let now = new Date();

              //if(json['msgtype']!="MsgBaseInfo")
              console.log("==>("+now.getTime()/1000+")["+json['msgtype']+"] \t:"+bbuffer.length+" =>"+e.target.result.length);
              
              if(json.hasOwnProperty('msgtype')) {
                switch(json['msgtype']){
                  case 'MsgSvrList':
                    state.SvrList = json.svrlist;
                    break;
                  case 'MsgExecCommand':
                    let date = new Date();
                    let seperator1 = ":";
                    let hh = date.getHours();
                    let mm = date.getMinutes();
                    let ss = date.getSeconds();
                    let currentdate = hh + seperator1 + mm + seperator1 + ss;

                    let from = json.from + " "+currentdate+"    Exec Command: " + json.command + "\n"
                    state.ExecResult += from
                    state.ExecResult += json.result;
                    state.ExecResult += '\n'
                    break;
                  }
              }
            }
            frd.readAsText(b, 'utf-8');
          } else {
            console.error(err);
          }
        });
      }
      reader.readAsArrayBuffer(message.data)
    },
    // mutations for reconnect methods
    SOCKET_RECONNECT(state, count) {
        //console.info(state, count)
        console.info("SOCKET_RECONNECT!");
    },
    SOCKET_RECONNECT_ERROR(state) {
        state.socket.reconnectError = true;
    },
  },
  actions: {
    SetStateAuth: function(context, visible) {
      context.commit("StateAuth", visible);
    }
  }
})

