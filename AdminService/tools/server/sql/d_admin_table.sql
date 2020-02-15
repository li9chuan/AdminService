/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50611
Source Host           : localhost:3306
Source Database       : d_admin_svr

Target Server Type    : MYSQL
Target Server Version : 50611
File Encoding         : 65001

Date: 2013-05-05 14:05:24
*/

drop database if exists d_admin_svr ;
create database d_admin_svr default character set utf8mb4;
use d_admin_svr;

DROP TABLE IF EXISTS `t_shard`;
CREATE TABLE `t_shard` (
  `f_shard` varchar(128) NOT NULL DEFAULT '',
  `f_host` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`f_shard`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS t_user;
CREATE TABLE `t_user`
(
   `f_uid`                 bigint(20) unsigned NOT NULL DEFAULT 0,
   `f_name`                varchar(64) NOT NULL DEFAULT " ",
   `f_pwd`                 varchar(64) NOT NULL DEFAULT " ",
   `f_insert_time`         timestamp DEFAULT CURRENT_TIMESTAMP COMMENT '记录生成时间',
  PRIMARY KEY (`f_uid`), INDEX (`f_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DROP TABLE IF EXISTS t_pow;
CREATE TABLE `t_pow`
(
   `f_uid`                 bigint(20) unsigned NOT NULL DEFAULT 0,
   `f_shard`               varchar(64) NOT NULL DEFAULT " ",
   `f_power`               bigint(20) unsigned NOT NULL DEFAULT 0 COMMENT '功能权限(pb_enum)',
  PRIMARY KEY (`f_uid`, `f_shard`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DROP TABLE IF EXISTS t_log;
CREATE TABLE `t_log`
(
   `f_id`                  int(10) unsigned NOT NULL AUTO_INCREMENT,
   `f_uid`                 bigint(20) unsigned NOT NULL COMMENT '',
   `f_opt`                 int(10) unsigned NOT NULL DEFAULT 0 COMMENT '',
   `f_memo`                varchar(2048) NOT NULL DEFAULT " ",
   `f_timestamp`           timestamp DEFAULT CURRENT_TIMESTAMP COMMENT '记录生成时间',
  PRIMARY KEY (`f_id`), INDEX (`f_uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


