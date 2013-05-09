
// takes a list of names of modules and a callback to call when ALL modules are loaded
function ModuleLoader(modules, callback) {
	var load = function (mods) {
		if (mods.length === 0) {
			console.log("ModuleLoader done");
			callback.callback();
		}
		Microsoft.Maps.loadModule(_.first(mods),{callback:function () {
			load (_.rest(mods));
		}});
	};
	load(modules);
}

Microsoft.Maps.moduleLoaded('ModuleLoader');