//���������Զ���Ӱ�Ȩ��Ϣ
document.body.oncopy = function ()
{
    setTimeout(
        function ()
        {
            var text = clipboardData.getData("text");
            if (text)
            {
                text = text + "\r\nԭ�ĳ��ԡ�����������ת���뱣��ԭ�����ӣ�"+location.href;
                clipboardData.setData("text", text);
            }
        },
        100
    )
}

//����΢��һ����ע
switch(channelId){
case 2 :
var uids="1660387637,1805180280,1646422590,1751790232,1824436584";
var ww="592";
break
case 38621 :
var uids="1660387637,1805180280,1646422590,1751790232,1824436584";
var ww="592";
break
case 44 :
var uids="1660387637,1805180280,1646422590,1824436584,1751790232";
//�ٷ����������ϲܣ������������
var ww="592";
break
case 46 :
var uids="1660387637,1805180280,1646422590,1824436584,1751790232";
var ww="592";
break
case 5548 :
var uids="1660387637,2588335650,1805180280,1179327887,1489442263";
var ww="602";
break
case 111824 :
var uids="1660387637,2587786742,1805180280,1179327887,1489442263";
var ww="602";
break
case 122 :
var uids="1660387637,2588335650,1805180280,1438318454,1655204372";
var ww="602";
break
case 9726 :
var uids="1660387637,2588335650,1805180280,1179327887,1765830541";
var ww="602";
break
case 45 :
var uids="1660387637,2588335650,1805180280,1731278122,1277139753";
var ww="602";
break
case 111760 :
var uids="1660387637,2588335650,1805180280,1731278122,1277139753";
var ww="602";
break
case 111761 :
var uids="1660387637,2588335650,1805180280,1179327887,1489442263";
var ww="602";
break
case 30707 :
var uids="1660387637,2588335650,1805180280,1179327887,1489442263";
var ww="602";
break
case 9725 ://��ȫ����΢����ҵ��������Ԭ�󡢹���
var uids="1660387637,2588335650,1805180280,1179327887,1420652245";
var ww="602";
break
case 9236 :
var uids="1660387637,2101395054,1805180280,1646422590,1949341930";
var ww="602";
break
case 40380 :
var uids="1660387637,2101395054,1805180280,1646422590,1949341930";
var ww="602";
break
case 31421 :
var uids="1660387637,2101395054,1805180280,1949341930,2252809697";
var ww="602";
break
case 5597 :
var uids="1660387637,2101395054,1805180280,1949341930,1253370227";
var ww="602";
break
case 118069 :
var uids="1660387637,2240113520,1805180280,1330738430,2198986263";
var ww="602";
break
case 38820 :
var uids="1660387637,2240113520,1805180280,1330738430,2198986263";
var ww="602";
break
case 118243 :
var uids="1660387637,2240113520,1805180280,1330738430,1927024321";
var ww="602";
break
case 118244 :
var uids="1660387637,2240113520,1805180280,1330738430,1066389432";
var ww="602";
break
case 118245 :
var uids="1660387637,2240113520,1805180280,1330738430,1066389432";
var ww="602";
break
case 118246 :
var uids="1660387637,2240113520,1805180280,1330738430,1797096627";
var ww="602";
break
case 120398 :
var uids="1660387637,2240113520,1805180280,1330738430,1797096627";
var ww="602";
break
//�����Ľڵ㡰��������
case 124020 :
var uids="1660387637,2240113520,1646422590,1179327887,1330738430";
var ww="602";
break
//��������
case 30659 :
var uids="1660387637,2240113520,1646422590,1179327887,1330738430";
var ww="602";
break
//�ʼǱ�/��������
case 111719 :
var uids="1660387637,2240113520,1646422590,1179327887,1330738430";
var ww="602";
break
//̨ʽ��/һ�����
case 111721 :
var uids="1660387637,2240113520,1179327887,1330738430,1066389432";
var ww="602";
break
//һ�����
case 111718 :
var uids="1660387637,2240113520,1179327887,1330738430,1066389432";
var ww="602";
break
//ƽ����ԣ�
case 125540 :
var uids="1660387637,2240113520,1179327887,1330738430,1797096627";
var ww="602";
break
//ͶӰ��ʾ��
case 111749 :
var uids="1660387637,2240113520,1179327887,1330738430,1756625801";
var ww="602";
break
//����վ��
case 122475 :
var uids="1660387637,2240113520,1179327887,1330738430,1066389432";
var ww="602";
break
//OA��ӡ��
case 31387 :
var uids="1660387637,2240113520,1179327887,1330738430,1756625801";
var ww="602";
break
//���������
case 119022 :
var uids="1660387637,2240113520,1179327887,1330738430,1066389432";
var ww="602";
break
//�ƶ������������������������⡢�ܴ�������
case 124182 :
var uids="1660387637,1805180280,1646422590,1051344342,2987635524";
var ww="592";
break
default:
var uids="1660387637,1805180280,1646422590,1751790232,1824436584";
var ww="592";
}
var weibo='<div class="box" id="m-wb"><div class="hd"><h3>΢���Ƽ�</h3><!-- div class="act"><iframe id="previewmc" src="http://follow.v.t.qq.com/index.php?c=follow&a=quick&name=chinabyte&style=5&t=1325213214061&f=1" width="125" height="24" frameborder="0" scrolling="no" allowtransparency="true" style="height:26px;display:inline;"></iframe><iframe src="http://open.qzone.qq.com/like?url=http%3A%2F%2Fuser.qzone.qq.com%2F1517281698&type=button_num&width=200&height=25&style=2" allowtransparency="true" scrolling="no" border="0" frameborder="0" style="height:25px;border:none;overflow:hidden;"></iframe></div --></div><div class="bd"><iframe width="678" height="185" frameborder="0" scrolling="no" src="http://widget.weibo.com/relationship/bulkfollow.php?language=zh_cn&uids='+uids+'&wide=1&color=ffffff,FFFFFF,0082CB,666666&showtitle=0&showinfo=1&sense=0&verified=1&count=5&refer='+encodeURIComponent(location.href)+'&dpc=1"></iframe></div></div>';
if(uids){
	$('.mod:first').before(weibo);
}
