//多标签box
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
//我要挑错
function submitFeedback() {
	document.getElementById("title").value = articleTitle;
	document.getElementById("url").value = document.location.href;
	document.getElementById("articleId").value = referId;
	document.getElementById("sitemapId").value = channelId;
	document.getElementById("feedbackForm").submit();
}
/* 显示层并向全局注册 */
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

//订阅
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
			alert("请输入正确的邮件地址");
			return false;
		}
	}else{
		alert("请填写邮件地址");
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
		alert("暂时没有可订阅的杂志");
		return false;
	}
	if(c==0){
		alert("请至少选择一项");
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

/* 无线合作 李智 */
function sc1(){
document.write("<li ><a href=\"http://iot.10086.cn:12080/search/search.jsp?sid=1310662320477&keyword=%E6%97%A0%E7%BA%BF%E5%9F%8E%E5%B8%82&go.x=0&go.y=0\">中国移动大力发展“无线城市”让智能生活不再遥远</a></li>");
}
function sc2(){
document.write("<li><a href=\"http://iot.10086.cn/\" title=\"打造物联网智能家居时代\"><img src=\"http://image.tianjimedia.com/w/a/20110920/lizhi.jpg\" width=\"120\" height=\"75\" /></a> ");
document.write("<h5><a href=\"http://iot.10086.cn/\" title=\"打造物联网智能家居时代\">打造物联网智能家居时代</a></h5></li>");
}
function sc3(){
document.write("<li ><a href=\"http://iot.10086.cn/2012-10-07/1336962224592.html\">中移动成立物联网公司 开发二维码产品</a></li>");
document.write("<li ><a href=\"http://iot.10086.cn/2012-10-08/1336962224781.html\">物联网应用在教育领域风起云涌</a></li>");
}
function sc4(){
document.write("<li ><a href=\"http://iot.10086.cn\">手机扫描二维码 一扫便可知天下</a></li>");
document.write("<li ><a href=\"http://iot.10086.cn/ewm/\">利用WiFi打造智能家居“物联网”</a></li>");
}
function sc5(){
document.write("<li ><a href=\"http://iot.10086.cn\">中国移动物联网专网专号 打造专用高速路</a></li>");
}