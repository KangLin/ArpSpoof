var _BFD = {};_BFD.addEvent=function(a,b,c){if(a.addEventListener){a.addEventListener(b,c,false)}else{if(a.attachEvent){a.attachEvent("on"+b,function(){c.call(a)})}else{a["on"+b]=c}}};_BFD.removeEvent=function(a,b,c){if(a.removeEventListener){a.removeEventListener(b,c,false)}else{if(a.detachEvent){a.detachEvent("on"+b,function(){c.call(a)})}else{a["on"+b]=null}}};_BFD.createElement=function(d,a){var c=document.createElement(d);if(a){for(var b in a){if(a.hasOwnProperty(b)){if(b==="class"||b==="className"){c.className=a[b]}else{if(b==="style"){c.style.cssText=a[b]}else{c.setAttribute(b,a[b])}}}}}return c};_BFD.loadScript=function(a,b){setTimeout(function(){var c=_BFD.createElement("script",{src:a,type:"text/javascript"});if(c.readyState){_BFD.addEvent(c,"readystatechange",function(){if(c.readyState==="loaded"||c.readyState==="complete"){if(b){b()}_BFD.removeEvent(c,"readystatechange",arguments.callee)}})}else{_BFD.addEvent(c,"load",function(){if(b){b()}_BFD.removeEvent(c,"load",arguments.callee)})}document.getElementsByTagName("head")[0].appendChild(c)},0)};_BFD.getByAttribute=function(f,g,a){var b=[],a=(a)?a:document,d=a.getElementsByTagName("*");for(var c=d.length-1;c>=0;c--){var e=d[c];if(f==="className"||f==="class"){if(e.className===g){b.send(e)}}else{if(e.getAttribute(f)===g){b.send(e)}}}return b};_BFD.getByClass=function(b,a){if(typeof document.getElementsByClassName==="function"){a=(a)?a:document;return Array.prototype.slice.call(a.getElementsByClassName(b))}else{return _BFD.getByAttribute("className",b,a)}};_BFD.getById=function(a){if(typeof a==="string"&&!!a){return document.getElementById(a)}};_BFD.loadCss=function(a){var b=_BFD.createElement("link",{href:a,rel:"stylesheet",type:"text/css"});document.getElementsByTagName("head")[0].appendChild(b)};


_BFD.remRepeat = function(arr, repeat_ids, num) {
	var res = [];
	var j = 0;
	for(var i = 0; i<arr.length; i++){
		var item_id = arr[i];
			
		if (!repeat_ids[item_id]) {
			res.push(arr[i]);
			repeat_ids[item_id] = true;
			j++;
		}
		if (j>=num) {
			break;
		}
	}
	return res;
}
_BFD.repeat_ids = {};
_BFD.getByClass = function(oParent, sClass){
	var aEle=oParent.getElementsByTagName('*');
	var re=new RegExp('(^|\\s)'+sClass+'(\\s|$)', 'i');
	var aResult = [],aTmp = [],i = 0,j = 0;
	for(i=0;i<aEle.length;i++){
		if(re.test(aEle[i].className)){
			aResult.push(aEle[i]);
			break;
		}
	}
	return aResult;
}
_BFD.bfd_show_vav = function(rec_result,req_id,banner_id){
	var parent_div = _BFD.getByClass(document,"mod")[0];
	//var parent_div = $(".mod")[0];

	var bfd_div = document.createElement("div");
	bfd_div.className = "box";
	bfd_div.id = "bfd_goods_box";

	var html = "<div class='hd hd_box'><h3>浏览过本文的还浏览过</h3><div id='bfd_logo4'><a href='http://www.baifendian.com' target='_blank'></a></div></div>";

	html += "<div class='bd'><ul class='list'>";

	for(var i = 0,len = rec_result.length<10?rec_result.length:10;i<len;i++){
		html += "<li><a href='"+rec_result[i].url+"'>"+rec_result[i].name+"</a></li>";
	}
	html +="</ul></div>";

	bfd_div.innerHTML = html;
	if(parent_div){
		var oBox = _BFD.getByClass(parent_div,"box");
		var clearfix = _BFD.getByClass(parent_div,"clearfix")[0];
		if(oBox.length === 2){
			oBox[1].style.display = "none";
		}
		parent_div.insertBefore(bfd_div,clearfix);
	}
}

_BFD.loadCss("http://static1.baifendian.com/service/bite/bite_style.css");

_BFD.loadScript(('https:' == document.location.protocol?'https://ssl-static':'http://static')+'.baifendian.com/api/2.0/bcore.min.js',function(){
	new $Core(function(){
		//获取className方法（最新）

		//获取类别
		var _cat = [],
			_cat_url = [],
			_cat_tree = [];
		var getcatid = document.getElementById("navbar");
		//var getcatclass = getByClass(getcatid,"wnav")[0];
		var getcata = getcatid.getElementsByTagName('a');
		for(var i=1;i<getcata.length;i++){
				_cat.push(getcata[i].innerHTML);
				_cat_url.push(getcata[i].href);
				_cat_tree[i-1] = [];
				_cat_tree[i-1].push(_cat[i-1]);
				_cat_tree[i-1].push(_cat_url[i-1]);
		}
		_cat_tree.pop();

		//获取文章名称
		var _pname = document.getElementById("artibodyTitle").innerHTML;
		//获取item_id

		var _url = (self.location.href).split("?")[0];
		var _artical_id =(/\/([0-9_]*?)\./).exec(_url)[1];
		
		//获取文章的发表时间
		var m = document.getElementById("main")
		var _time= _BFD.getByClass(m,"date")[0].innerHTML;
		_time=_time.replace(/-/g,"\/");
		//var _time= $("#main .date").innerHTML.replace(/-/g,"\/");

		var _ptime = (+new Date(_time))/1000;
		
		
		var Tools = $Core.tools.Tools;

		var _category = Tools.removeRepeatArr(_cat);


		//var _exp = BFD_ITEM_INFO.expire;
		var _customid = "Cbite"; //正式帐号；

		this.options.p_t 	= "dt";//页面类型
		this.options.p_p = "rec";
		this.options.p_s = _pname;//页面标签
			
			//初始化页面参数
		this.options.cid = _customid;
		this.options.uid = this.options.sid;
			
		if(_artical_id.indexOf("_") === -1&&!isNaN(+_artical_id)&&(/[\u4e00-\u9fa5]/.test(_pname))&&(/[\u4e00-\u9fa5]/.test(_category[0]))&&!(/<.*>/.test(_category[0]))){
					//添加商品
			
			var add_item = new $Core.inputs.AddItem(_artical_id);
			add_item.title = _pname;
			add_item.cat = _cat;
			add_item.url = _url;
			add_item.ptime = _ptime;

			this.send(add_item);

			var page_view = new BCore.inputs.PageView();
			page_view.p_s = _pname;//页面标签
			this.send(page_view);

			var  _cat_name = _cat[_cat.length-1];
			var add_cat = new BCore.inputs.AddCat(_cat_name);
				add_cat.name = _cat_name;
				add_cat.url =  _cat_url[_cat.length-1];
				add_cat.ptree = new BCore.inputs.JObject().toString(_cat_tree);
			this.send(add_cat);
			this.send(new BCore.inputs.VisitCat(_cat_name));
		}

		this.send(new BCore.inputs.Visit(_artical_id.split("_")[0]));

		var Filter = BCore.recommends.Filter;
		var filterCat = new Filter("cat","in",[_cat[_cat.length-1]]);


		var rfmt = new $Core.inputs.JObject();
		rfmt.iid = "$id";
		rfmt.name = "$title";
		rfmt.url = "$url";
		BCore.recommends.Recommend.prototype.fmt = rfmt.toString();

		var rec_vav = new BCore.recommends.MRecVAV(_artical_id.split("_")[0],10);

		rec_vav.flt = filterCat.toString()

		rec_vav.cat = _cat[_cat.length-1];
	
		rec_vav.p_bid = '5E7D8DA6_7C12_5954_0388_3463C887C60C';
		
		this.send(rec_vav, function(json){
				
			var rec_result = [];
			var req_id = "";
			var banner_id = "5E7D8DA6_7C12_5954_0388_3463C887C60C";
			if (json&&json[3]!=null) {
				rec_result = json[3];
				req_id = json[2];
			}
			
			if(_BFD.bfd_show_vav && rec_result && rec_result.length > 0){

					_BFD.bfd_show_vav(rec_result, req_id, banner_id);
					var lis = document.getElementById('bfd_goods_box').getElementsByTagName('li');
					//var lis = $("#bfd_goods_box ul li");

					for(var i=0,len = lis.length;i<len;i++){
						var _a = lis[i].getElementsByTagName("a");
						for(var j = 0;j<_a.length;j++){
							Tools.bind(_a[j],"click",(function(x,iid){
								return function (){
									new BCore(function(){
										var fb = new $Core.inputs.FeedBack(req_id);
										fb.p_on = x + 1;
										fb.p_bid = banner_id;
										fb.iid = iid;
										this.send(fb);
									});
								}
							})(i, rec_result[i].iid));
						}
					}
					if(document.getElementById("bfd_goods_box")){
						BCore.exts.BannerShow.prototype.push(document.getElementById("bfd_goods_box"),banner_id);
					}

			}

		});


		
	});
});
