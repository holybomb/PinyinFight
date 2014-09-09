%%%----------------------------------- 
%%% @Module  : data_gamequest 
%%% @Author  : YANG.JIN 
%%% @Email   : 84959751@qq.com 
%%% @Created : 2013-9-2 16:43:33 
%%% @Description: 自动生成 
%%%----------------------------------- 
-module(data_gamequest). 
-export([get/1]). 
-include("record.hrl"). 
 
get(1)-> 
#data_gamequest{ 
	id=1,skey="pdsh_1",sname="圣旨除妖",ac_level=1,max_level=11,ac_job=0,times=0,ac_pre_quest="",follow_quest="pdsh_2",accept_npc="pdsh_jvlingshe_1",submit_npc="pdsh_lijing_1",stype=1,accept_desc="玉帝下旨，收降此处诸妖。你可愿助我等一臂之力。",submit_desc="果然身手了得。片刻之间便将群妖打散。",uncomdesc="平顶密林驱散诸妖",completedesc="找托塔天王复命",finash_map="d_pingdingshan_1",reward_money=20,reward_exp=200 
}; 
get(2)-> 
#data_gamequest{ 
	id=2,skey="pdsh_2",sname="再入魔窟",ac_level=1,max_level=11,ac_job=0,times=0,ac_pre_quest="pdsh_1",follow_quest="",accept_npc="pdsh_lijing_1",submit_npc="pdsh_muzha_1",stype=1,accept_desc="妖魔有一头领，你如降服此妖孽，必在玉帝面前为你请功。",submit_desc="此妖乃老君之小童，生性暴烈。今被降服，实乃天下苍生之福。",uncomdesc="去降服金角大王",completedesc="向木吒复命",finash_map="d_pingdingmilin_1",reward_money=100,reward_exp=200 
}; 
 
 
get(_Id) -> 
[]. 
