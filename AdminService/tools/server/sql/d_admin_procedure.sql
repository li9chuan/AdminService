use d_admin_svr ;

DELIMITER ;;

-- t_shard
drop procedure if exists _t_shard_load;
drop procedure if exists _t_shard_insert;
drop procedure if exists _t_shard_update;
drop procedure if exists _t_shard_holdtk;
drop procedure if exists _t_shard_delete;

create procedure _t_shard_load()
begin
	select f_shard, f_host from t_shard;
end;;

create procedure _t_shard_insert(
	in af_shard varchar(128),
	in af_host varchar(128)
	)
begin
	insert into t_shard(  f_shard, f_host ) values ( af_shard, af_host );
end;;

create procedure _t_shard_update(
	in af_shard varchar(128),
	in af_host varchar(128)
	)
begin
	update t_shard set f_host=af_host where f_shard=af_shard;
end;;

create procedure _t_shard_delete( in af_shard varchar(128) )
begin
    delete from t_shard where f_shard=af_shard;
end;;





DELIMITER ;











