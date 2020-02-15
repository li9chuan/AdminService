<template>
    <div>
        <div class="crumbs">
            <el-breadcrumb separator="/">
                <el-breadcrumb-item>
                    <i class="el-icon-lx-cascades"></i> 服务列表
                </el-breadcrumb-item>
            </el-breadcrumb>
        </div>
        <div class="container">
            <el-table :data="SvrList" border class="table" ref="multipleTable" header-cell-class-name="table-header" @selection-change="handleSelectionChange">
                <el-table-column type="selection" width="55" align="center"></el-table-column>
                <el-table-column prop="shard" label="片区" sortable align="center" :filters="filterShard" :filter-method="filterHandler"></el-table-column>
                <el-table-column prop="name" label="服务名" sortable :filters="filterName" :filter-method="filterHandler"></el-table-column>
                <el-table-column prop="sid" label="SvrID" sortable></el-table-column>
                <el-table-column prop="ip" label="IP" sortable :filters="filterIP"></el-table-column>
            </el-table>

            <div class="handle-box crumbs">
                <el-input v-model="query.exec" placeholder="命令" class="handle-input mr10"></el-input>
                <el-button type="primary" icon="el-icon-search" @click="handleSearch">执行</el-button>
                <el-button type="primary" icon="el-icon-delete" @click="handleClear">清空</el-button>
                <el-button type="primary" icon="el-icon-refresh" @click="handleRefresh">刷新</el-button>
            </div>

            <el-input type="textarea" rows="9" v-model="ExecResult"></el-input>
        </div>
    </div>
</template>

<script>
import {mapState, mapMutations} from 'vuex'
export default {
    name: 'basetable',
    data() {
        return {
            query: {
                exec: ''
            },
            filterShard: [],
            filterName: [],
            filterIP: [],
            svrSelection: [],
            delList: []
        };
    },
    computed: {
        ...mapState(['SvrList','ExecResult']),
    },
    created() {
    },
    watch:{
        SvrList(newval){
            this.filterShard = []
            this.filterName  = []
            this.filterIP    = []
            let fshard = {}
            let fname  = {}
            let fip    = {}

            for(let i=0; i<newval.length; ++i){
                let val = newval[i]
                fshard[val.shard] = 1
                fname[val.name] = 1
                fip[val.ip] = 1
            }

            for(let key in fshard){ 
                this.filterShard.push({text:key, value:key})
            }
            for(let key in fname){ 
                this.filterName.push({text:key, value:key})
            }
            for(let key in fip){ 
                this.filterIP.push({text:key, value:key})
            }
        }
	},
    methods: {
        ...mapMutations(['ClearExecResult']),
        handleSearch() {
            if( this.svrSelection.length>0 ){
                let MsgExecCommand = { 
                    'msgtype': 'MsgExecCommand',
                    'svrlist': [],
                    'command': this.query.exec
                };

                for( let i=0; i<this.svrSelection.length; ++i ){
                    let svr = this.svrSelection[i]
                    MsgExecCommand.svrlist.push( { shard: svr.shard, name: svr.name, sid: svr.sid } )
                }
                this.$socket.send( JSON.stringify(MsgExecCommand) );
            }
        },
        // 多选操作
        handleSelectionChange(val) {
            this.svrSelection = val
        },
        handleClear(val) {
            this.ClearExecResult()
        },
        handleRefresh(val) {
            this.$socket.send('{"msgtype":"MsgSvrList"}');
        },
        filterHandler(value, row, column) {
            const property = column['property'];
            return row[property] === value;
        }
    }
};
</script>

<style scoped>
.handle-box {
    margin-bottom: 20px;
}

.handle-select {
    width: 120px;
}

.handle-input {
    width: 300px;
    display: inline-block;
}
.table {
    width: 100%;
    font-size: 14px;
}
.red {
    color: #ff0000;
}
.mr10 {
    margin-right: 10px;
}
.table-td-thumb {
    display: block;
    margin: auto;
    width: 40px;
    height: 40px;
}
</style>
