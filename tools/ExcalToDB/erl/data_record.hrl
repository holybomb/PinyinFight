%%%----------------------------------- 
%%% @Module  : data_record.hrl 
%%% @Author  : YANG.JIN 
%%% @Email   : 84959751@qq.com 
%%% @Created : 2013-9-2 16:43:33 
%%% @Description: 别修改，自动生成的！ 
%%%----------------------------------- 
%%data_levelinfo 
-record(data_levelinfo, { 
	id,levelup_exp 
}). 
%%data_levelinfo 
%%data_arminfo 
-record(data_arminfo, { 
	id,sname,icon,description,subtype,issell,isincrease,usearea,quality,joblimit,minlevellimit,maxlevellimit,goldprice,yuanbaoprice,zhangongprice,rongyuprice,sellprice,gemslot,hp,mp,strength,intelligence,stunt,attacknormal,defensenormal,attackmagic,defensemagic,attackskill,defenseskill,hitrate,crit,hp_step,an_step,dn_step,am_step,dm_step,as_step,ds_step 
}). 
%%data_arminfo 
%%data_gamequest 
-record(data_gamequest, { 
	id,skey,sname,ac_level,max_level,ac_job,times,ac_pre_quest,follow_quest,accept_npc,submit_npc,stype,accept_desc,submit_desc,uncomdesc,completedesc,finash_map,reward_money,reward_exp 
}). 
%%data_gamequest 
