//���ǩbox
function boxs(id,defaultItem,how){
$(id+" .hd ul li").bind(how, function(){
  var idt = $(id+" .hd ul li").index(this);
  $(id+" .hd ul li:eq("+idt+")").addClass("selected").siblings().removeClass("selected");
  $(id+" .bd div.tab-panel:eq("+idt+")").show().siblings().hide();
  $(id).css("background","#fff");
});
}

function tabs(tabTit,on,tabCon){
	$(tabCon).each(function(){
		$(this).children().eq(0).show();
	});
	$(tabTit).children().hover(function(){
		$(this).addClass(on).siblings().removeClass(on);
		var index = $(tabTit).children().index(this);
		$(tabCon).children().eq(index).show().siblings().hide();
	});
}

var rssurl=""
var opiniontitle=document.title;
var opinionurl=document.location.href;
//��Ҫ����
function submitFeedback() {
	document.getElementById("title").value = articleTitle;
	document.getElementById("url").value = document.location.href;
	document.getElementById("articleId").value = referId;
	document.getElementById("sitemapId").value = channelId;
	document.getElementById("feedbackForm").submit();
}
/* ��ʾ�㲢��ȫ��ע�� */
function showLayer(id){
	 $("#"+id).fadeIn(100);
}
function hideLayer(id){
	 $("#"+id).fadeOut(100);
}

var szs = new Array( '12','14','16' );
var startSz = 1;
function zoomDoc( inc ) {
	var sz = startSz;
	sz += inc;
	if ( sz < 0 ) sz = 0;
	if ( sz > 2 ) sz = 2;
	startSz = sz;
    $('#main-article p').css('font-size',szs[sz]+'px');
}

//����
function submitSubscribe(){
	if(check()){
		document.getElementById("subscribeForm").submit();
	}else{
		return false;
	}
}
function submitUnSubscribe(){
	if(check()){
		document.getElementById("ac").value="unSubscribe"
		document.getElementById("subscribeForm").submit();
	}else{
		return false;
	}
}
function check(){
	var email = document.getElementById("email");
	var groupIds = document.getElementsByName("mailGroupId");
	if(email!=null && email.value!=null && email.value!=""){
		if(isEmail(email.value)){
		}else{
			alert("��������ȷ���ʼ���ַ");
			return false;
		}
	}else{
		alert("����д�ʼ���ַ");
		return false;
	}

	var c = 0;
	if(groupIds!=null && groupIds.length>0){
		for(i=0;i<groupIds.length;i++){
		if(groupIds[i].checked == true){
			c = 1; 
		}
	  }
	}else{
		alert("��ʱû�пɶ��ĵ���־");
		return false;
	}
	if(c==0){
		alert("������ѡ��һ��");
		return false;
	}
	return true;
}
function isEmail(strEmail) {
	if (strEmail.search(/^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/) != -1){
		return true;
	}else{
		return false;
	}
}
//End	

/* ���ߺ��� ���� */
function sc1(){
document.write("<li ><a href=\"http://iot.10086.cn:12080/search/search.jsp?sid=1310662320477&keyword=%E6%97%A0%E7%BA%BF%E5%9F%8E%E5%B8%82&go.x=0&go.y=0\">�й��ƶ�������չ�����߳��С������������ңԶ</a></li>");
}
function sc2(){
document.write("<li><a href=\"http://iot.10086.cn/\" title=\"�������������ܼҾ�ʱ��\"><img src=\"http://image.tianjimedia.com/w/a/20110920/lizhi.jpg\" width=\"120\" height=\"75\" /></a> ");
document.write("<h5><a href=\"http://iot.10086.cn/\" title=\"�������������ܼҾ�ʱ��\">�������������ܼҾ�ʱ��</a></h5></li>");
}
function sc3(){
document.write("<li ><a href=\"http://iot.10086.cn/2012-10-07/1336962224592.html\">���ƶ�������������˾ ������ά���Ʒ</a></li>");
document.write("<li ><a href=\"http://iot.10086.cn/2012-10-08/1336962224781.html\">������Ӧ���ڽ������������ӿ</a></li>");
}
function sc4(){
document.write("<li ><a href=\"http://iot.10086.cn\">�ֻ�ɨ���ά�� һɨ���֪����</a></li>");
document.write("<li ><a href=\"http://iot.10086.cn/ewm/\">����WiFi�������ܼҾӡ���������</a></li>");
}
function sc5(){
document.write("<li ><a href=\"http://iot.10086.cn\">�й��ƶ�������ר��ר�� ����ר�ø���·</a></li>");
}