    /*
      Elysia Genomics Editor - This editor allows users to select regions and paint them with gene properties

      Copyright (C) 2009  Daniel Reiter Horn, Ilmari Heikkinen

      This program is free software; you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation; either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program; if not, write to the Free Software
      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

      http://www.gnu.org/copyleft/gpl.html
    */
    var windowWidth=1024;
    var windowHeight=768;
    var zMin=-99999999999999.;
    var DEFAULT_CURSOR = 'default';
    var MOVE_TO_CURSOR = 'url(moveto.png) 9 9, move';
    var TARGET_CURSOR = 'crosshair';
    var SELECT_CURSOR = 'pointer';
    var stringWhitespaceOnly=function (st){ // NOT USED IN FORM VALIDATION
        return /^\s*$/.test(st);
    };
    ///debugPrint prints to the console if there is a console, but does not error if the console is unavailable
    var debugPrint=function (x) {
        if(typeof(console)!='undefined') {
            if (console) {
                if (console.hasOwnProperty('log')) {
                    console.log(x);
                }
            }
        }else {
            debugPrint=function(x){};
        }
    };
    var mapEmpty=function (a) {
        for (var i in a) {
            return false;
        }
        return true;
    };
    ///This function checks if the number of items in a {} hashtable numbers exactly 1.
    var onlyOneElement=function (a) {
        var onlyOneElement=false;
        for (var i in a) {
            if (onlyOneElement) {
                onlyOneElement=false;
                break;
            }else {
                onlyOneElement=true;
            }
        }
        return onlyOneElement;
    };
    ///This function tells if a point is near a corner of a bounding box. Returns the corner number in clockwise order starting from 0. -1 means it is not near a corner
    var nearCorner=function(point, boundingBox) {
        var testConstant=3;
        testConstant*=testConstant;
        if (point[0]-testConstant<boundingBox[0]&&point[1]-testConstant<boundingBox[1]) {
            return 0;
        }

        if (point[0]+testConstant>boundingBox[2]&&point[1]-testConstant<boundingBox[1]) {
            return 1;
        }

        if (point[0]+testConstant>boundingBox[2]&&point[1]+testConstant>boundingBox[3]) {
            return 2;
        }

        if (point[0]-testConstant<boundingBox[0]&&point[1]+testConstant>boundingBox[3]) {
            return 3;
        }
        return -1;
    };
    ///This function tells if a point is near a edge of a bounding box. Returns the edge number in clockwise order starting from 0. -1 means it is not near a corner
    var nearEdge=function(point, boundingBox) {
        var testConstant=3;
        testConstant*=testConstant;
        if (point[0]-testConstant<boundingBox[0]) {
            return 0;
        }
        if (point[1]-testConstant<boundingBox[1]) {
            return 1;
        }
        if (point[0]+testConstant>boundingBox[2]) {
            return 2;
        }
        if (point[1]+testConstant>boundingBox[3]) {
            return 3;
        }
        return -1;
    };

    var M = {
      rotation : function(rotation) {
        return CanvasSupport.tRotationMatrix(rotation);
      },

      scaling : function(x, y) {
        return CanvasSupport.tScalingMatrix(x, y);
      },

      translation : function(x, y) {
        return CanvasSupport.tTranslationMatrix(x, y);
      }
    };

    var V = {
      rotate : function(v, rotation) {
        return V.multiply(v, M.rotation(rotation));
      },

      add : function(v, u) {
        return [u[0] + v[0], u[1] + v[1]];
      },

      multiply : function(v, matrix) {
        return CanvasSupport.tMatrixMultiplyPoint(matrix, v[0], v[1]);
      }
    };

    ///global variable indicating what the zindex of the current frontmost div is--this allows clicking to put one in front of the other
    var divMaxZIndex=100;
    ///this is the number of lobe property panes that have been brought up. Since we want to assure each has a unique id we will need a unique value for each one
    var lobeDivCount=0;
    var PerGeneValueEntryBox=function(editor,getGeneValue,setGeneValue,geneUpdateFunctionName) {
        var perGeneValueEntryBox={};
        perGeneValueEntryBox.genes=[];
        perGeneValueEntryBox.initializeGene=function(gene) {
            var newValue=getGeneValue(gene);
            perGeneValueEntryBox.genes[perGeneValueEntryBox.genes.length]=gene;
            if (perGeneValueEntryBox.hasOwnProperty('minValue')) {
                if( newValue<perGeneValueEntryBox.minValue) {
                    perGeneValueEntryBox.minValue=newValue; 
                }
                if( newValue>perGeneValueEntryBox.maxValue) {
                    perGeneValueEntryBox.maxValue=newValue; 
                }
            }else{ 
                perGeneValueEntryBox.minValue=newValue;
                perGeneValueEntryBox.maxValue=newValue;
            }
        };
        perGeneValueEntryBox.getDescriptionString=function(){ 
            if (perGeneValueEntryBox.hasOwnProperty('minValue')) {
                if (perGeneValueEntryBox.minValue<perGeneValueEntryBox.maxValue) {
                    return "["+perGeneValueEntryBox.minValue+","+perGeneValueEntryBox.maxValue+"]";
                }
                return ""+perGeneValueEntryBox.minValue;
            }else return "";
        };
        perGeneValueEntryBox.update=function(newValue) {
            bracket="[".charAt(0);
            rbracket="]".charAt(0);                        
            var minValue=0;
            var maxValue=0;
            if (newValue.charAt(0)==bracket) {
                newValue=newValue.substr(1,newValue.length-1);
                if (newValue.charAt(newValue.length-1)==rbracket) {
                    newValue=newValue.substr(0,newValue.length-1);
                }
                newValues=newValue.split(',');
                minValue=parseFloat(newValues[0]);
                if (newValues.length>1) {
                    maxValue=parseFloat(newValues[1]);
                    if (maxValue<minValue) {
                        var tmp=minValue;
                        minValue=maxValue;
                        maxValue=tmp;
                    }
                }else maxValue=minValue;
            }else {
                minValue=maxValue=parseFloat(newValue);
            }
            var leng=perGeneValueEntryBox.genes.length;
            var first=true;
            for (var i=0;i<leng;++i) {
                var gene=perGeneValueEntryBox.genes[i];
                if (getGeneValue(gene)<minValue||getGeneValue(gene)>maxValue) {
                    //perform undo
                    var undoFunctor=function (myGene) {
                        var gene=myGene;
                        var oldValue=getGeneValue(myGene);
                        return function() {
                            setGeneValue(gene,oldValue);
                            if (gene.hasOwnProperty(geneUpdateFunctionName)) {
                                gene[geneUpdateFunctionName](oldValue);
                            }

                        };
                    };
                    var redoFunctor=function (myGene) {
                        var gene=myGene;
                        var oldValue=getGeneValue(myGene);
                        return function() {
                            if (oldValue<minValue) {
                                setGeneValue(gene,minValue);
                                if (gene.hasOwnProperty(geneUpdateFunctionName)) {
                                    gene[geneUpdateFunctionName](minValue);
                                }
                            }
                            if (oldValue>maxValue) {
                                setGeneValue(gene,maxValue);
                                if (gene.hasOwnProperty(geneUpdateFunctionName)) {
                                    gene[geneUpdateFunctionName](maxValue);
                                }
                            }
                        };
                    };
                    editor.context.performedAction(redoFunctor(gene),undoFunctor(gene));
                    redoFunctor(gene)();
                    if (!first) {
                        editor.context.coalesceUndos();
                    }
                    first=false;
                }
            }
        };
        return perGeneValueEntryBox;
    };
    ///this function brings up a lobe property pane for the hashtable (id->lobe) of selected lobes passed in
    var LobeDiv = function(editor) {
        var context=editor.context;
        var div = document.createElement("div");
        document.body.appendChild(div);
        div.style.left="10px";
        div.style.top="20px";
        div.style.width="288px";
        div.style.height="512px";
        div.style.padding="0.5em";
        div.style.position="absolute";
        div.style.border="solid 10px #10107c";
        div.style.backgroundColor="#000008";

        lobeDivCount+=1;
        div.id="ldiv"+lobeDivCount;
        div.onmousedown=function(){div.raiseDiv();};
        div.raiseDiv=function (){            
            div.style.zIndex=divMaxZIndex++;
        };
        jQuery("#"+div.id).draggable().resizable();
        div.innerHTML='<p class="alignleft">Lobe Properties</p><p class="alignright"><a href="javascript:LobeDiv.close('+"'"+div.id+"'"+')">X</a></p><div style="clear:both;"/><br/><br/>';
        div.genes=[];
        var first=true;
        var lobeNames='';
        var minAge=PerGeneValueEntryBox(editor,
                                        function(gene){return gene.minAge;},
                                        function(gene,newMinAge){gene.minAge=newMinAge;},
                                        'setChildrenAgeIndicator');
        var maxAge=PerGeneValueEntryBox(editor,
                                        function(gene){return gene.maxAge;},
                                        function(gene,newMaxAge){gene.maxAge=newMaxAge;},
                                        'setChildrenAgeIndicator');
        var internalProteins={};
        var externalProteins={};
        var proteinCodes={}
        var geneuids={};
        for (var uid in context.selection) {
            var lobe=context.selection[uid];    
            if (!geneuids[lobe.gene.uid]) {
                geneuids[lobe.gene.uid]=lobe.gene;
                div.genes[div.genes.length]=lobe.gene;
                minAge.initializeGene(lobe.gene);
                maxAge.initializeGene(lobe.gene);
                var numInternalProteins=lobe.gene.internal_proteins.length;
                var i;
                for (i=0;i<numInternalProteins;i+=1){
                    var code=lobe.gene.internal_proteins[i].protein_code;
                    if (code in Elysia.Genome.Effect) {
                        internalProteins[Elysia.Genome.Effect[code]]=code;
                        proteinCodes[Elysia.Genome.Effect[code]]=code;
                    }else {
                        internalProteins["Unknown"+code]=code;
                        proteinCodes["Unknown"+code]=code;
                    }
                }

                var numExternalProteins=lobe.gene.external_proteins.length;
                for (i=0;i<numExternalProteins;i+=1){
                    var code=lobe.gene.external_proteins[i].protein_code;
                    if (code in Elysia.Genome.Effect) {
                        externalProteins[Elysia.Genome.Effect[code]]=code;
                        proteinCodes[Elysia.Genome.Effect[code]]=code;
                    }else {
                        externalProteins["Unknown"+code]=code;
                        proteinCodes["Unknown"+code]=code;
                    }
                }

                if (!first) {
                    lobeNames+=',';
                }
                lobeNames+=lobe.gene.name;
                first=false;
            }
        }
        var proteinGet=function(proteinCode,geneproteins) {
            var leng=geneproteins.length;
            var retval=0.0;
            for (var i=0;i<leng;i+=1) {
                if (geneproteins[i].protein_code==proteinCode && geneproteins[i].density)
                    retval+=geneproteins[i].density;
            }
            return retval;
        };
        var proteinGetAll=function(proteinCode,geneproteins) {
            var leng=geneproteins.length;
            var retval=[];
            for (var i=0;i<leng;i+=1) {
                if (geneproteins[i].protein_code==proteinCode)
                    retval[retval.length]=geneproteins[i];
            }
            return retval;
        };
        var proteinSet=function(proteinCode,geneproteins,input) {
            var leng=geneproteins.length;
            for (var i=0;i<leng;i+=1) {
                if (geneproteins[i].protein_code==proteinCode)
                    geneproteins[i].density=input;
            }
        };
        var proteinFind=function(proteinCode,geneproteins) {
            var found=false;
            var leng=geneproteins.length;
            for (var i=0;i<leng;i+=1) {
                if (geneproteins[i].protein_code==proteinCode)
                    found=true;
            }
            return found;            
        };
        var proteinEnable=function(proteinCode,geneproteins,input) {
            var leng=geneproteins.length;
            if (!proteinFind(proteinCode,geneproteins)) {
                var prot=new Elysia.Genome.Protein();
                prot.protein_code=proteinCode;
                prot.density=input;
                geneproteins.push(prot);
            }
        };
        var proteinDisable=function(proteinCode,geneproteins) {
            var leng=geneproteins.length;
            for (var i=leng-1;i>=0;i-=1) {
                if (geneproteins[i].protein_code==proteinCode) {
                    if (i!=geneproteins.length-1) {
                        geneproteins[i]=geneproteins[geneproteins.length-1];                        
                    }
                    geneproteins.length=geneproteins.length-1;
                }
            }
        };
        for (var internal in internalProteins) {
            internalProteins[internal]=PerGeneValueEntryBox(editor,
                                                            function() {
                                                                var code=internalProteins[internal];
                                                                return function(gene) {
                                                                    return proteinGet(code,gene.internal_proteins);};
                                                            }(),
                                                            function() {
                                                                var code=internalProteins[internal];
                                                                return function(gene,value) {
                                                                    
                                                                    proteinSet(code,gene.internal_proteins,value);
                                                                };
                                                            }(),
                                                            "");
            div[internal]=internalProteins[internal].update;
        }
        for (var external in externalProteins) {
            externalProteins[external]=PerGeneValueEntryBox(editor,
                                                            function() {
                                                                var code=externalProteins[external];
                                                                return function(gene) {
                                                                    return proteinGet(code,gene.external_proteins);};
                                                            }(),
                                                            function() {
                                                                var code=externalProteins[external];
                                                                return function(gene,value) {
                                                                    
                                                                    proteinSet(code,gene.external_proteins,value);
                                                                };
                                                            }(),
                                                            "");
            div[external]=externalProteins[external].update;
        }
        div.minAge=minAge.update;
        div.maxAge=maxAge.update;
        var defaultControls=[
//          ['makeNewLobe','function'],
            ['name','string',{value:lobeNames,size:26,reverse:false}],
            ['createLobeInputFrom','string',{size:13,reverse:true}],
            ['createLobeOutputTo','string',{size:13,reverse:true}],
            ['selectOutputLobes','function'],
            ['selectInputLobes','function']
            //,['minAge','string',{value:minAge.getDescriptionString(),onblur:true,reverse:true}]
            //,['maxAge','string',{value:maxAge.getDescriptionString(),onblur:true,reverse:true}]
           ];
        var geneAddCount=0;
        for (internal in internalProteins) {
            var tmp=internalProteins[internal];
            var allHaveGene=true;
            for (var i=0;i<div.genes.length;i+=1) {
                tmp.initializeGene(div.genes[i]);
                if (!proteinFind(proteinCodes[internal],div.genes[i].internal_proteins)) {
                    allHaveGene=false;
                }
            }
            defaultControls[defaultControls.length]=[internal,'string',{value:tmp.getDescriptionString(),size:4,onblur:true,reverse:true,newline:true}];
            geneAddCount+=1;
            var makeProduceName='makeAllProduce';
            if (allHaveGene)
                makeProduceName='deleteProtein';
            if (geneAddCount>1) {
                makeProduceName+=geneAddCount;
            }
            //defaultControls[defaultControls.length]=[makeProduceName,'function',{varName:makeProduceName,newline:false,br:true}];
            div[makeProduceName]=function(ahg) {
                var code=proteinCodes[internal];
                var allHaveIt=ahg;
                return function() {
                    var listsToDelete=[];
                    var itemsWithNothing={};
                    for (var i=0;i<div.genes.length;i+=1) {
                        var proteins=proteinGetAll(code,div.genes[i]);
                        if (proteins.length==0) {
                            var prot=new Elysia.Genome.Protein;
                            prot.density=0;
                            prot.protein_code=code;
                            proteins[0]=prot;
                            itemsWithNothing[i]=true;
                        }else {
                            itemsWithNothing[i]=false;
                        }
                        listsToDelete[listsToDelete.length]=proteins;
                    }
                    var undo=function() {
                        for (var i=0;i<div.genes.length;i+=1) {   
                            proteinDisable(code,div.genes[i].internal_proteins);
                            for (var j=0;j<listsToDelete[i].length;j+=1){
                                div.genes[i].internal_proteins[div.genes[i].internal_proteins.length]=listsToDelete[i][j];
                            }
                        }
                    };
                    var redo=function() {
                        for (var i=0;i<div.genes.length;i+=1) {                        
                            if (itemsWithNothing[i]||allHaveIt)
                                proteinDisable(code,div.genes[i].internal_proteins);
                            
                        }
                    };
                    if (allHaveIt) {
                        context.performedAction(undo,redo);
                        redo();
                    }else {
                        context.performedAction(redo,undo);
                        undo();                            
                    }
                };
            }(allHaveGene);
        }            
        for (external in externalProteins) {
            var tmp=externalProteins[external];
            var allHaveGene=true;
            for (var i=0;i<div.genes.length;i+=1) {
                tmp.initializeGene(div.genes[i]);
                if (!proteinFind(proteinCodes[external],div.genes[i].external_proteins)) {
                    allHaveGene=false;
                }
            }            
            defaultControls[defaultControls.length]=[external,'string',{value:tmp.getDescriptionString(),size:4,onblur:true,reverse:true}];            
            geneAddCount+=1;
            var makeProduceName='makeAllProduce';
            if (!allHaveGene)
                makeProduceName='deleteProtein';
            if (geneAddCount>1) {
                makeProduceName+=geneAddCount;
            }
            div[makeProduceName]=function(ahg) {
                var code=proteinCodes[external];
                var allHaveIt=ahg;
                return function() {
                    var listsToDelete=[];
                    var itemsWithNothing={};
                    for (var i=0;i<div.genes.length;i+=1) {
                        var proteins=proteinGetAll(code,div.genes[i]);
                        if (proteins.length==0) {
                            var prot=new Elysia.Genome.Protein;
                            prot.density=0;
                            prot.protein_code=code;
                            proteins[0]=prot;
                            itemsWithNothing[i]=true;
                        }else {
                            itemsWithNothing[i]=false;
                        }
                        listsToDelete[listsToDelete.length]=proteins;
                    }
                    var undo=function() {
                        for (var i=0;i<div.genes.length;i+=1) {   
                            proteinDisable(code,div.genes[i].external_proteins);
                            div.genes[i].external_proteins=div.genes[i].external_proteins.concat(listsToDelete[i]);
                        }
                    };
                    var redo=function() {
                        for (var i=0;i<div.genes.length;i+=1) {                        
                            if (itemsWithNothing[i]||allHaveIt)
                                proteinDisable(code,div.genes[i].external_proteins);
                            
                        }
                    };
                    if (allHaveIt) {
                        context.performedAction(undo,redo);
                        redo();
                    }else {
                        context.performedAction(redo,undo);
                        undo();                            
                    }
                };
            }(allHaveGene);
    
            //defaultControls[defaultControls.length]=[makeProduceName,'function',{varName:makeProduceName,newline:false,br:true}];
        }

        div.controlPanel = new GuiConfig({
          object : div,
          container : div,
          controls : defaultControls
        });
        div.controlPanel.show();
        div.createLobesOn=function(str, lobeCreator, compatPredicate){
            if (stringWhitespaceOnly(str)) {
                var ycount=0;
                var divlen=div.genes.length;
                for (var i =0; i< divlen;i+=1) {
                    var gene=div.genes[i];
                    lobeCreator(gene,[0,ycount,64,32+ycount]);
                    ycount+=48;
                }
            }else {
                var regexMatch= function (lobe) {
                    if (lobe.gene&&compatPredicate(lobe)) {
                        return lobe.gene.name==str;
                    }
                    return false;
                };
                var divlen=div.genes.length;
                for (var i =0; i< divlen;i+=1) {
                    editor.createLobesOnPredicate(regexMatch,function(bbox) {
                        lobeCreator(div.genes[i],bbox);
                    });
                }
                //only place it on matching nodes
            }
        };
        div.createLobeOutputTo=function(string) {
            var lobeOutputCreator=function(gene,bbox) {
                var lobe= new LobeOutput(editor,gene);
                lobe.setBoundingBox(bbox);
                return editor.makeNewSelectable(lobe);
            };
            div.createLobesOn(string,lobeOutputCreator,function(l){return !l.isAxon;});                     
        };
        div.createLobeInputFrom=function(string) {
            var lobeInputCreator=function(gene,bbox) {
                var lobe= new LobeInput(editor,gene);
                lobe.setBoundingBox(bbox);
                return editor.makeNewSelectable(lobe);
            };   
            div.createLobesOn(string,lobeInputCreator,function(l){return l.isAxon;});         
        };
        div.selectOutputLobes=function() {
            for (var index=0;index<div.genes.length;index+=1) {
                div.genes[index].selectOutputs();
            }            
        };
        div.selectInputLobes=function() {
            for (var index=0;index<div.genes.length;index+=1) {
                div.genes[index].selectInputs();
            }            
        };
        div.name=function(newName){
            newNames=newName.split(',');
            for (var index=0;index<div.genes.length;index+=1) {
                var divgene=div.genes[index];
                var oldName=divgene.name;
                var nName=newNames[newNames.length-1];
                if (newNames.length>index) {
                    nName=newNames[index];
                }
                divgene.setChildrenName(nName);
                
                context.performedAction(function(){var lnName=nName;var gene=divgene;return function(){gene.setChildrenName(lnName);};}(),
                                        function(){var oName=oldName;var gene=divgene;return function(){gene.setChildrenName(oName);};}());
                if (index!=0) {
                    context.coalesceUndos();
                }
            }
        };
        return div;
    };
    ///this function disposes of a lobe with unique id passed in. It removes the draggable property then destroys the div
    LobeDiv.close=function(name) {
        div=document.getElementById(name);
        jQuery("#"+div.id).draggable("destroy");
        div.parentNode.removeChild(div);
    };
    /// This function makes a new context. Context holds transient editor state such as the currently held selection and the undo list. This state will not be serialized to disk when the save function is invoked.
    var Context = function(genomeEditor) {
        var context={};
        context.mGenomeEditor=genomeEditor;
        context.selection={};
        context.groupUndoIndexStack=[];
        context.groupRedoIndexStack=[];
        context.mUndoList=[];
        context.mRedoList=[];
        context.mCurUndoCounter=0;
        context.toggleSelect = function(s) {
            if (this.selection.indexOf(s.id) == -1)
                this.select(s);
            else
                this.deselect(s);
        };
        ///this gets the front item that is not selected in the passed in list
        context.frontNonselected = function(sel) {
            var maxZ=zMin;
            var frontObjects=[];
            sel.forEach(function(item) {
                if (item.hasOwnProperty('uid')&&!context.selection[item.uid]) {
                    var zi=item.getZIndex();
                    if (zi>maxZ||frontObjects.length==0) {
                        frontObjects=[item];
                        maxZ=zi;
                    }
                }
            });
            return frontObjects;
        };
        ///this gets the front item that is selected in the passed in list
        context.frontSelected = function(sel) {
            var maxZ=zMin;
            var frontObjects=[];
            sel.forEach(function(item) {
                if (item.hasOwnProperty('uid')&&context.selection[item.uid]) {
                    var zi=item.getZIndex();
                    if (zi>maxZ||frontObjects.length==0) {
                        frontObjects=[item];
                        maxZ=zi;
                    }
                }
            });
            return frontObjects;
        };
        ///this gets the next item in a cycle from front to back starting with the currently selected guy (or else the front one if nothing is selected)
        context.nextItemsInLine = function(sel) {
            var maxZ=zMin;
            var returnAny=true;
            var oldselection=sel;
            sel=new Array(0);
            var item;
            oldselection.forEach(function(item) {
                if(item.hasOwnProperty('uid')) {
                    sel.push(item);
                }
                });
            sel=sel.sort(function(a,b){var az=a.getZIndex(); var bz=b.getZIndex(); if (az==bz) return az.uid<bz.uid;return az>bz;});
            var sellen=sel.length;
            var i=0;
            while (i<sellen) {
                item=sel[i];
                if (context.selection[item.uid]) {
                    returnAny=false;
                    break;
                }
                i+=1;
            }
            if(returnAny) {
                return context.frontNonselected(sel);
            }
            debugPrint("returning before "+i+"/"+sel.length);
            if(i==0) {
                debugPrint("(z="+sel[sel.length-1].getZIndex()+")");
                return [sel[sel.length-1]];
            }
            debugPrint("(z="+sel[i-1].getZIndex()+")");
            return [sel[i-1]];
        };
        ///this clears any selected item and makes it so no items are selected
        context.clearSelection = function() {
            for (var item in this.selection) {
                this.deselect(this.selection[item]);
            }
        };
        ///this selects the given item that inherits from Selectable
        context.select = function(s) {
            if (s.hasOwnProperty('uid')) {
                s.root.dispatchEvent({type : 'select', canvasTarget: s});
                this.selection[s.uid]=s;
                s.select();
            }
        };
        ;///this deselects the given item that inherits from Selectable
        context.deselect = function(s) {
            if (s.uid in this.selection) {
                if (s.hasOwnProperty('uid')) {
                    s.root.dispatchEvent({type : 'deselect', canvasTarget: s});
                    delete this.selection[s.uid];
                }
            }
            s.deselect();
        };
        ///this is a future use function for making lobe tarets
        context.setLobeTarget= function(point)  {
            
        };
        ///this takes the top two undos in the stack and turns them into a single action that must be undone or redone together
        context.coalesceUndos = function () {
            if (this.mUndoList.length>1 && this.mRedoList.length>1) {
                var lastUndo=this.mUndoList.pop();
                var penultimateUndo=this.mUndoList.pop();
                var lastRedo=this.mRedoList.pop();
                var penultimateRedo=this.mRedoList.pop();
                this.performedAction(function(){penultimateRedo();lastRedo();},function(){lastUndo();penultimateUndo();});
            }
        };
        ///this indicates that an action has been performed and it should be stored in the undo/redo list
        context.performedAction= function(redoFunction,undoFunction) {
            if (this.mCurUndoCounter!=this.mUndoList.length) {
                var leng=this.mUndoList.length;
                var rleng=this.mRedoList.length;
                var count=0;
                for (var i=this.mCurUndoCounter;i<leng;i+=1) {
                    //debugPrint ("Pushing redo "+(leng-1-count)+ " to undo stack");
                    this.mUndoList.push(this.mRedoList[leng-1-count]);
                    this.mRedoList.push(this.mUndoList[leng-1-count]);
                    count+=1;
                }
            }
            this.mUndoList.push(undoFunction);
            this.mRedoList.push(redoFunction);
            this.mCurUndoCounter=this.mUndoList.length;
        };
        ///this finds the last function that is on the undo list and executes it, effectively undoing an action
        context.undo= function () {      
            if (this.mCurUndoCounter>0) {
                this.mCurUndoCounter--;
                this.mUndoList[this.mCurUndoCounter]();
            }
        };
        ///this finds the next function to be redone if there are any and executes it.
        context.redo = function() {
            if (this.mCurUndoCounter<this.mUndoList.length) {
                this.mRedoList[this.mCurUndoCounter]();
                this.mCurUndoCounter++;
            }
        };
        return context;
    };
    ///this keeps track of the maximum lobe z value of the lobes so that new lobes come out in front of other lobes
    var currentMaxZ = 2;
    ///this function gets a unique lobe identifier for this session so that two lobes are not given the same name
    var getUID= (function() {
            var start=0;
            return function() {
                start+=1;
                return start;
            };
        })();
var Gene = function(editor,baseElysiaGenomeGene) {
        var th=baseElysiaGenomeGene;
        th.name="gene_"+getUID();
        th.uid=getUID();       
        th.minAge=0.0;
        th.maxAge=1.0;
        var newProtein=function(geneName,density){
            var prot=new Elysia.Genome.Protein();
            prot.density=density;
            prot.protein_code=geneName;
            return prot;

        };
        th.external_proteins.push(newProtein(Elysia.Genome.Effect.GROW_NEURON,1.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.BASE_BRANCHINESS,1.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.TIP_BRANCHINESS,1.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.BASE_THRESHOLD,1.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.TIP_THRESHOLD,1.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.TREE_DEPTH,5.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.RECEPTIVITY_TIME,1.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.LEARNING_RESPONSIVENESS,1.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.INHIBITION,0.0));
        th.internal_proteins.push(newProtein(Elysia.Genome.Effect.AGGRESSIVE_DEVELOPMENT,0.0));
        
        th.findLobesWithThisGene=function() {
            return editor.childNodes.filter(function(s) {
                try {
                    if(s.hasOwnProperty('gene')) {
                        if (s.gene.uid==th.uid) {
                            return true;
                        }
                    }
                }catch (e) {return false;}
                return false;
            });
        };
        th.selectOutputs=function() {
            this.findLobesWithThisGene().forEach(function(s) {
                if(s.isAxon) {
                    editor.context.select(s);
                }
            });
        };
        th.selectInputs=function() {
            this.findLobesWithThisGene().forEach(function(s) {
                if(!s.isAxon) {
                    editor.context.select(s);
                }
            });
        };
        th.setChildrenName=function(newName) {
            this.name=newName;
            this.findLobesWithThisGene().forEach(function(s) {
                s.name.text=newName;
            });
        };
        th.setChildrenAgeIndicator=function(newAge) {
            this.findLobesWithThisGene().forEach(function(s) {
               s.recomputeAgeIndicator();
            });
        };
    return th;
};
    /// the selectable class is an interface that any object which wishes to be selected must inherit from. Some functions should be overridden in the subclass
    var Selectable = Klass (CanvasNode, {
        initialize: function(editor) {
            CanvasNode.initialize.call(this);
            this.uid=getUID();
            this.mSelected=false;
            this.mEditor=editor;
            {
                var th=this;
                this.addEventListener('keydown',editor.keyDown)
                this.addEventListener('keyup',editor.keyUp)
                this.addEventListener('mousedown',function(ev) {
                        if (editor.clickedSelected(ev)) {
                            editor.enableMoveOnMouseUp=true;
                            editor.mouseDragDownHandler.bind(editor)(ev);
                        }else {
                            editor.enableMoveOnMouseUp=false;
                            editor.mouseDownHandler.bind(editor)(ev);
                        }
                    },false);
                this.addEventListener('drag',function(ev) {
                        if(editor.enableMoveOnMouseUp) {
                            editor.mouseDragMoveHandler.bind(editor)(ev);
                        }else {
                            editor.mouseDragHandler.bind(editor)(ev);
                        }
                    },false);
            }
        },
        getBoundingBox:function() {
            return CanvasNode.getBoundingBox.call(this);
        },
        ///returns true unless the node is hidden or otherwise unselectable
        isSelectable:function() {
            return true;
        },
        select:function(){
            this.mSelected=true;
        },
        deselect:function(){
            this.mSelected=false;
        },
        isSelected:function() {
            return this.mSelected;
        },
        getZIndex: function() {
            return zMin;
        },
        getOrigin:function() {
            var retval=new Array(3);
            retval[0]=this.x;
            retval[1]=this.y;
            retval[2]=this.getZIndex();
            return retval;
        },
        setOrigin:function(origin) {
            this.x=origin[0];
            this.y=origin[1];
        },
        setBoundingBox:function(bb) {
            this.x=bb[0];
            this.y=bb[1];
        }
    });
    ///Lobe is a Selectable that represents a gene area on the map of the creature. Each lobe has a number of propreties that will affect the development of a given creature
    var Lobe = Klass(Selectable, {
        initialize: function(editor,gene) {
                //call the Selectable initialize function which sets whether this is selected to false, etc
                Selectable.initialize.bind(this)(editor);
                //make the meat of the gene: the region it occupies in the creature
                this.gene=gene;

                this.lobe=new Rectangle(64,32, {
                        stroke : false,
                        stroke : [0,0,0,0],
                        fill : [256,256,256,0.0625],
                        visible : true,
                        zIndex : currentMaxZ+=2
                    });
                this.lobe.x+=5;
                this.lobe.y+=274;
                this.append(this.lobe)
                //make an canvas TextNode that prints the name of the gene on the lobe
                this.name=new TextNode(gene.name);//,{align:'center',baseline:'hanging'});
                this.name.align='center';
                this.name.baseline='bottom';
                this.name.cx=0;
                this.name.cy=this.lobe.height;
                this.name.maxWidth=this.lobe.width;
                this.lobe.append(this.name);
                this.minAge=0.0;
                this.maxAge=1.0;
           
                this.ageSpan=new TextNode('[0,1]');//,{align:'center',baseline:'hanging'});
                this.ageSpan.align='center';
                this.ageSpan.baseline='bottom';
                this.ageSpan.cx=0;
                this.ageSpan.cy=this.lobe.height/2;
                this.ageSpan.maxWidth=this.lobe.width;
                this.lobe.append(this.ageSpan);
                this.recomputeAgeIndicator();
            },
            recomputeAgeIndicator:function() {
                var gmna=this.gene.minAge;
                var gmxa=this.gene.maxAge;
                var mna=this.minAge;
                var mxa=this.maxAge;
                gmna=Math.round(gmna*100)/100;
                gmxa=Math.round(gmxa*100)/100;
                mna=Math.round(mna*100)/100;
                mxa=Math.round(mxa*100)/100;
                if (mna==0&&mxa==1) {
                    this.ageSpan.text="["+gmna+","+gmxa+"]";
                }else if (this.gene.minAge==0&&this.gene.maxAge==1) {
                    this.ageSpan.text="{"+mna+","+mxa+"}";
                }else {
                    this.ageSpan.text='['+gmna+"{"+mna+","+mxa+"}"+gmxa+"]";
                }
                var full=this.visibleColor();
                var empty=this.vanishColor();
                var pctBad=this.getPercentOutsideAgeRange();
                if (pctBad>0)  {
                    pctBad*=.5;
                    pctBad+=.5;
                }
                var pctGood=1.0-pctBad;
                var col=[full[0]*pctGood+empty[0]*pctBad,
                         full[1]*pctGood+empty[1]*pctBad,
                         full[2]*pctGood+empty[2]*pctBad,
                         full[3]*pctGood+empty[3]*pctBad];
                if (!this.isSelected()) {
                    col[3]*=.25;
                    if (pctBad!=0) {
                        if (!this.mEditor.context.mGenomeEditor.DrawAgeInactiveLobes) {
                            col[3]=0;
                        }
                    }
                }
                this.lobe.fill=col;
                //debugPrint("Updating age "+this.mEditor.context.age+"("+this.mEditor.context.mGenomeEditor.Age+")"+" for uid "+this.uid+ " col is "+col+" cbox "+this.mEditor.context.mGenomeEditor.DrawAgeInactiveLobes);
            },
            isSelectable:function() {
                return this.mEditor.context.mGenomeEditor.DrawAgeInactiveLobes||this.getPercentOutsideAgeRange()==0;
            },
            getPercentOutsideAgeRange:function() {
                var gmna=this.gene.minAge;
                var gmxa=this.gene.maxAge;
                var mna=this.minAge;
                var mxa=this.maxAge;
                if (gmna>mna) mna=gmna;
                if (gmxa<mxa) mxa=gmxa;
                if (mxa>1.0) {
                    debugPrint("max age > 1.0");
                    mxa=1.0;
                }
                if (mna<0.0) {
                    debugPrint("min age < 0.0");
                    mna=0.0;
                }
                var age=this.mEditor.context.mGenomeEditor.Age;
                if (age>1.0) {
                    debugPrint("age > 1.0");
                    age=1.0;
                }
                if (age<0.0) {
                    debugPrint("age < 0.0");
                    age=0.0;
                }
                var pctBad=0.0;
                if (age>mxa) {
                    pctBad=(age-mxa)/(1.0-mxa);
                }else if (age<mna) {
                    pctBad=(mna-age)/mna
                }
                return pctBad;
            },
            getMinAge:function() {
                var retval=this.minAge;
                if (this.minAge>this.gene.minAge)
                    return this.gene.minAge;
                return this.minAge;
            },
            getMaxAge:function() {
                var retval=this.maxAge;
                if (this.maxAge<this.gene.maxAge)
                    return this.gene.maxAge;
                return this.maxAge;
            },
            setMinAge:function(mA) {
                this.minAge=mA;
                this.recomputeAgeIndicator();
                return this.minAge==this.getMinAge();
            },
            setMaxAge:function(mA) {
                this.maxAge=mA;
                this.recomputeAgeIndicator();
                return this.maxAge==this.getMaxAge();
            },
            ///returns the z value (for selection and viewing order)
            getZIndex: function() {
                return this.lobe.zIndex;
            },
            ///gets the bounding box of the lobe by returning the this.lobe's bounding box
            getBoundingBox: function() {
                var bb=this.lobe.getBoundingBox();
                var retval=new Array(4);
                retval[0]=bb[0]+this.lobe.x;
                retval[1]=bb[1]+this.lobe.y;
                retval[2]=bb[2]+this.lobe.x;
                retval[3]=bb[3]+this.lobe.y;
                return retval;
            },
            ///select this lobe and paint it differently
            select:function() {
                Selectable.select.call(this);
                this.recomputeAgeIndicator();
            },
            ///deselect this lobe and paint it differently
            deselect:function() {
                Selectable.deselect.call(this);
                this.recomputeAgeIndicator();
            },
            ///gets the x,y origin of the painted rectangle
            getOrigin:function() {
                var retval=new Array(3);
                retval[0]=this.lobe.x;
                retval[1]=this.lobe.y;
                retval[2]=this.getZIndex();
                return retval;
            },
            ///resets the x,y origin of the painted rectangle
            setOrigin:function(origin) {
                this.lobe.x=origin[0];
                this.lobe.y=origin[1];
                this.lobe.zIndex=origin[2];
            },
            ///sets the bounding box of the entire gene--this specifies the top left corner and bottom right corner. This function can effectively move a lobe by moving its topleft and bottom right corners.
            setBoundingBox:function(bb) {
                var x1 = Math.min(bb[0], bb[2]);
                var x2 = Math.max(bb[0], bb[2]);
                var y1 = Math.min(bb[1], bb[3]);
                var y2 = Math.max(bb[1], bb[3]);

                this.lobe.x=x1;
                this.lobe.y=y1;
                this.lobe.width=x2-x1;
                this.lobe.height=y2-y1;
                this.name.cy=this.lobe.height;
                this.name.maxWidth=this.lobe.width;

                this.ageSpan.cy=this.lobe.height/2;
                this.ageSpan.maxWidth=this.lobe.width;
            }
        });
        var LobeOutput=Klass(Lobe,{initialize:function(editor,gene) {
            Lobe.initialize.bind(this)(editor,gene);
            this.isAxon=true;
        },
        visibleColor:function() {
           return [128,128,128,.5];
        },
        vanishColor:function() {
           return [128,0,0,.0625];
        }
                              });
        var LobeInput=Klass(Lobe,{initialize:function(editor,gene) {
            Lobe.initialize.bind(this)(editor,gene);
            this.isAxon=false;
        },
        visibleColor:function() {
           return [0,128,128,.5];
        },
        vanishColor:function() {
           return [64,64,0,.06125];
        }
                              });
    ///The editor holds the state of the canvas, the context, and all active lobes as well as current transient mouse state and selection boxes
    var Editor = Klass(CanvasNode, {
      bgColor : 'rgb(0,0,0)',
      bgOpacity : 0.15,

      ///this sets the state of the editor and binds all the mouse functions, etc
      initialize : function(genomeEditor,saveString,isFather) {
        CanvasNode.initialize.call(this);
        this.saveString=saveString;
        this.x=-windowWidth/2;
        this.y=-windowHeight/2;
        this.context=Context(genomeEditor);
        var context=this.context;
        var sizeMultiplier=16;
        this.bg = new Rectangle(this.width*sizeMultiplier,this.height*sizeMultiplier);
          //this.bg.x=-this.width*sizeMultiplier/2;
          //this.bg.y=-this.height*sizeMultiplier/2;
        this.bg.fill = this.bgColor;
        this.bg.fillOpacity = this.bgOpacity;

        this.bgleft = new Rectangle(this.width*sizeMultiplier,this.height*sizeMultiplier*2);
        this.bgleft.x=-this.width*sizeMultiplier;
        this.bgleft.y=-this.height*sizeMultiplier;
        this.bgleft.fill = [35,38,44,1.0];
        this.bgleft.fillOpacity = this.bgOpacity;

        this.bgup = new Rectangle(this.width*sizeMultiplier,this.height*sizeMultiplier);
        this.bgup.x=0;
        this.bgup.y=-this.height*sizeMultiplier;
        this.bgup.fill = [35,38,44,1.0];
        this.bgup.fillOpacity = this.bgOpacity;
        this.append(this.bgup);        
        this.append(this.bgleft);

        this.ignoreNextClick=false;
        var selectionStart, startX, startY, selectionDragPlace, nearWhichEdge=-1, nearWhichCorner=-1;
        var th = this;
        ///returns the list of objects that are inside the selection box or mouse click (depending on whether isSelectionBox is set).  MouseUpPoint is also used to better guess the end of the selection rectangle
        var objectsInside = function(rect, mouseUpPoint, isSelectionBox) {
          var x1 = Math.min(rect.cx, rect.x2);
          var x2 = Math.max(rect.cx, rect.x2);
          var y1 = Math.min(rect.cy, rect.y2);
          var y2 = Math.max(rect.cy, rect.y2);
          if (isSelectionBox) {
              if (mouseUpPoint[0]==0&&mouseUpPoint[1]==0) {
                  var delta=20;
                  if (x1-delta<0)
                      x1=0;
                  if (x2+delta>windowWidth)
                      x2=windowWidth;
                  if (y1-delta<0)
                      y1=0;
                  if (y2+delta>windowHeight) 
                      y2=windowHeight;
              }
              return th.childNodes.filter(function(s) {
                      try {
                          var bb=s.getBoundingBox();
                          if (bb[0]<bb[2]&&bb[1]<bb[3]) {
                              var minbb=[Math.max(bb[0],x1),
                                         Math.max(bb[1],y1),
                                         Math.min(bb[2],x2),
                                         Math.min(bb[3],y2)];
                              if (s.hasOwnProperty('uid')&&s.isSelectable()&&minbb[0]==bb[0]&&minbb[1]==bb[1]&&minbb[2]==bb[2]&&minbb[3]==bb[3]) {
                                  return true;
                              }else {
                                  return false;
                              }
                          }else return false;
                      }catch(e){return false;}
                  });
          }else {
              x1=x2=mouseUpPoint[0];
              y1=y2=mouseUpPoint[1];
              return th.childNodes.filter(function(s) {
                      try {
                          var bb=s.getBoundingBox();
                          if (bb[0]<bb[2]&&bb[1]<bb[3]) {
                              return x1>=bb[0]&&x1<=bb[2]&&y1>=bb[1]&&y1<=bb[3];
                          }else return false;
                      }catch(e){return false;}
                  });
          }
        };
        //actually make the visible selection rectangle, which is an actual CAKE object.
        this.selectRect = new Rectangle(0,0, {
          stroke : 1,
          strokeOpacity : 0.4,
          stroke : '#00ff00',
          fillOpacity : 0.1,
          fill : '#00ff00',
          visible : false,
          zIndex : 999
        });
        this.append(this.selectRect);
        this.clickedSelected=function(ev) {
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY);
          return context.frontSelected(objectsInside({cx:point[0],cy:point[1],x1:point[0],x2:point[1]},point,false)).length!=0;
        };
        ///Make the mouse drag down handler which handles what happens when a lobe is clicked to be dragged
        this.mouseDragDownHandler=function(ev) {
          th.performedDrag=false;
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          );
          nearWhichEdge=-1;
          nearWhichCorner=-1;
          var resizePossibility=onlyOneElement(context.selection);
          if (resizePossibility) {
              for (uid in context.selection) {
                  var bounds=context.selection[uid].getBoundingBox();
                  nearWhichCorner=nearCorner(point,bounds);
                  nearWhichEdge=nearEdge(point,bounds);
              
              }              
          }
          selectionStart = point;
          selectionDragPlace = point;
        };
        ///hash map of keyCodes which are currently pressed--this is used on firefox to reinject key events while the key is down
        this.keyPressed={};
        ///this function gets called every 10 milliseconds (triggered by itself) while a given keyCode is down until that key is "up"
        this.keyDownRepeat=function(keyCode) {
            if (th.keyPressed[keyCode]) {
                if (keyCode==65) {//left
                    th.parent.x-=10*th.scale;
                    if(th.parent.x<0) th.parent.x=0;
                }else if (keyCode==68) {//right
                    th.parent.x+=10*th.scale;
                }else if (keyCode==87) {//up
                    th.parent.y+=10*th.scale;
                }else if (keyCode==83) {//down
                    th.parent.y-=10*th.scale;
                    if(th.parent.y<0) th.parent.y=0;
                }else if (keyCode==81) {//zoom in
                    th.parent.scale*=1.01;
                }else if (keyCode==69) {//zoom out
                    th.parent.scale/=1.01;
                }
                th.after(5,function(){th.keyDownRepeat(keyCode)});
            }
        };
        ///mark that a key is no longer down in the keyPressed hash table
        this.keyUp=function(evt) {
            th.keyPressed[evt.keyCode]=false;
        };
        ///if the key is not down in the keyPressed hashtable, mark that a key is down in the keyPressed hash table and start a repeating keyDownRepeat function.
        this.keyDown=function(evt) {
            if (th.keyPressed[evt.keyCode]!=true) {
                th.keyPressed[evt.keyCode]=true;
                th.keyDownRepeat(evt.keyCode);
            }
        };
        ///What happens when a selection box or drag has been started from a click that began on a lobe itself
        this.mouseDragMoveHandler=function(ev) {
          th.performedDrag=true;
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          );
          var delta=[point[0]-selectionDragPlace[0],point[1]-selectionDragPlace[1]];
          for (uid in context.selection) {
              var origin=context.selection[uid].getOrigin();
              if (nearWhichCorner!=-1) {
                  var bb=context.selection[uid].getBoundingBox();
                  if (nearWhichCorner==0) {
                      bb[0]+=delta[0];
                      bb[1]+=delta[1];                      
                  }else if (nearWhichCorner==1){
                      bb[2]+=delta[0];
                      bb[1]+=delta[1];
                  }else if (nearWhichCorner==2) {
                      bb[2]+=delta[0];
                      bb[3]+=delta[1];
                  }else if (nearWhichCorner==3) {
                      bb[0]+=delta[0];
                      bb[3]+=delta[1];
                  }
                  context.selection[uid].setBoundingBox(bb);
              }else if (false&&nearWhichEdge!=-1) {
                  //IF YOU ADD THIS EDGE FUNCTION MAKE SURE TO ADD THE CORRESPONDING UNDO BELOW
                  debugPrint("dragEdge "+nearWhichEdge);
              }else {
                  origin[0]+=delta[0];
                  origin[1]+=delta[1];
                  context.selection[uid].setOrigin(origin);
              }
          }
          selectionDragPlace=point;
        };
        ///This function gets called when a click is made on the background canvas in preparation for either a select or a drag
        this.mouseDownHandler=function(ev) {
          th.performedDrag=false;
          th.enableMoveOnMouseUp=false;
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          );
          nearWhichEdge=-1;
          nearWhichCorner=-1;
          startX = this.root.mouseX;
          startY = this.root.mouseY;
          selectionStart = point;
          th.selectRect.x2 = th.selectRect.cx = point[0];
          th.selectRect.y2 = th.selectRect.cy = point[1];
        };
        ///This function gets called when a drag happens that started on a blank canvas area
        this.mouseDragHandler=function(ev) {
          th.performedDrag=true;
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(this.currentMatrix),
            this.root.mouseX, this.root.mouseY
          );
          if (selectionStart && !th.selectRect.visible) {
            var dx = startX - this.root.mouseX;
            var dy = startY - this.root.mouseY;
            var sqd = dx * dx + dy * dy;
            th.selectRect.visible = sqd > 81;
          }
          if (th.selectRect.visible) {
            th.selectRect.x2 = point[0];
            th.selectRect.y2 = point[1];
          }
        };
        this.bg.addEventListener('mousedown', this.mouseDownHandler, false);
        this.bg.addEventListener('drag', this.mouseDragHandler, false);
        this.bg.addEventListener('keydown',this.keyDown);
        this.bg.addEventListener('keyup',this.keyUp);
        ///commit a move command that moved one or more items to a final location (i.e. mouse released) and coalesce all the movements into a single undo command
        this.makeSelectionMoveUndo=function(delta) {
            var firstItem=true;
            if (nearWhichCorner!=-1){
              for (uid in th.context.selection) {                
                  var item=context.selection[uid];
                  var newbb=item.getBoundingBox();
                  var oldbb=item.getBoundingBox();
                  if (nearWhichCorner==0) {
                      oldbb[0]-=delta[0];
                      oldbb[1]-=delta[1];                      
                  }else if (nearWhichCorner==1){
                      oldbb[2]-=delta[0];
                      oldbb[1]-=delta[1];
                  }else if (nearWhichCorner==2) {
                      oldbb[2]-=delta[0];
                      oldbb[3]-=delta[1];
                  }else if (nearWhichCorner==3) {
                      oldbb[0]-=delta[0];
                      oldbb[3]-=delta[1];
                  }
                  if (!(oldbb[0]>0)) oldbb[0]=0;
                  if (!(oldbb[1]>0)) oldbb[1]=0;
                  if (!(oldbb[2]>0)) oldbb[2]=0;
                  if (!(oldbb[3]>0)) oldbb[3]=0;

                  if (!(newbb[0]>0)) newbb[0]=0;
                  if (!(newbb[1]>0)) newbb[1]=0;
                  if (!(newbb[2]>0)) newbb[2]=0;
                  if (!(newbb[3]>0)) newbb[3]=0;
                  item.setBoundingBox(newbb);
                  (function(nBb,oBb,cItem) {
                      var heapNewBb=nBb.slice(0);
                      var heapOldBb=oBb.slice(0);
                      var heapItem=cItem;
                      var undoFunction=function() {
                          heapItem.setBoundingBox(heapOldBb);
                      };
                      var redoFunction=function() {
                          heapItem.setBoundingBox(heapNewBb);
                      };
                      th.context.performedAction(redoFunction,undoFunction);                    
                  })(newbb,oldbb,item);
                  if (!firstItem) {
                      th.context.coalesceUndos(); 
                  }
                  firstItem=false;                  
              }
            }else if (false&&nearWhichEdge!=-1) {
                  //IF YOU ADD THIS EDGE UNDO FUNCTION MAKE SURE TO ADD THE CORRESPONDING RESIZER ABOVE
            }else {
              for (uid in th.context.selection) {                
                var item=th.context.selection[uid];
                var newOrigin=item.getOrigin();
                var oldOrigin=item.getOrigin();
                for (var i=0;i<delta.length&&i<newOrigin.length;i+=1) {
                    oldOrigin[i]=newOrigin[i]-delta[i];
                }
                  if(!(oldOrigin[0]>0)) oldOrigin[0]=0;
                  if(!(oldOrigin[1]>0)) oldOrigin[1]=0;
                  if(!(newOrigin[0]>0)) newOrigin[0]=0;
                  if(!(newOrigin[1]>0)) newOrigin[1]=0;
                item.setOrigin(newOrigin);
                (function(nOrigin,oOrigin,cItem) {
                    var heapNewOrigin=nOrigin.slice(0);
                    var heapOldOrigin=oOrigin.slice(0);
                    var heapItem=cItem;
                    var undoFunction=function() {
                        heapItem.setOrigin(heapOldOrigin);
                    };
                    var redoFunction=function() {
                        heapItem.setOrigin(heapNewOrigin);
                    };
                    th.context.performedAction(redoFunction,undoFunction);                    
                })(newOrigin,oldOrigin,item);
                if (!firstItem) {
                    th.context.coalesceUndos(); 
                }
                firstItem=false;
              }
            }
        };
        ///Handle the mouse being released which causes moves on items to be committed or items to be selected
        this.mouseupHandler = function(ev) {

            
          var point = CanvasSupport.tMatrixMultiplyPoint(
            CanvasSupport.tInvertMatrix(th.currentMatrix),
            th.root.mouseX, th.root.mouseY
          );
          if(th.enableMoveOnMouseUp&&th.performedDrag) {
              if (selectionStart) {
                  th.mouseDragMoveHandler(ev);
                  th.makeSelectionMoveUndo([point[0]-selectionStart[0],point[1]-selectionStart[1]]);
                  selectionStart=null;
              }
          }else {
            var doIgnoreNext=false;
            if (selectionStart||(th.root.mouseX>0&&th.root.mouseY>0&&!th.ignoreNextClick)) {
              var selectionBox=th.performedDrag;
              doIgnoreNext=true;//somehow we get 2 events for every legitimate event. This mitigates that factor.
              th.selectRect.visible = false;
              selectionStart = null;
              var selection = objectsInside(th.selectRect,point,selectionBox);
              if (ev.shiftKey) {
                if (!selectionBox) {
                    selection=context.frontNonselected(selection);
                }
                selection.forEach(context.select.bind(context));
              } else if (ev.altKey) {
                  if(!selectionBox) {
                      selection=context.frontSelected(selection);
                  }
                  selection.forEach(context.deselect.bind(context));
              } else {
                  if (!selectionBox) {
                      selection=context.nextItemsInLine(selection);
                  }
                  context.clearSelection();
                  selection.forEach(context.select.bind(context));
              }
            } else if (selectionStart && (ev.canvasTarget == th.selectRect || ev.canvasTarget == th.bg)) {
                context.setLobeTarget(point);
              th.selectRect.visible = false;
              selectionStart = null;
            }
            th.ignoreNextClick=doIgnoreNext; 
          }
        };
        this.addEventListener('rootChanged', function(ev) {
          if (ev.canvasTarget == this) {
            if (this.root)
              this.root.removeEventListener('mouseup', this.mouseupHandler, false);
            ev.relatedTarget.addEventListener('mouseup', this.mouseupHandler, false);
          }
        }, false);
        this.bg.zIndex = -100;
        this.messageLayer = new CanvasNode({
          zIndex : 1000,
          scale : 1 / this.scale
        });
        this.append(this.bg, this.messageLayer);
        this.addFrameListener(function() {
          if (false)
            this.cursor = MOVE_TO_CURSOR;
          else
            this.cursor = DEFAULT_CURSOR;
        });
      },
      deleteLobe : function () {
         var thus=this;
         var context=this.context;
         var first=true;
         for (var uid in context.selection) {
             var lb = context.selection[uid];
             var doIt=function (lobe) {
                 //need a separate isSelected variable per item in the loop to track selection of the objects individually
                 var isSelected=lobe.selected;
                 var undoFunction=function(){thus.append(lobe);if (isSelected) {context.select(lobe);}};
                 var redoFunction=function(){if(lobe.isSelected()) {isSelected=true;}context.deselect(lobe);thus.remove(lobe);};
                 redoFunction();
                 this.context.performedAction(redoFunction,undoFunction);
             };
             doIt(lb);
             if (!first){
                 this.context.coalesceUndos();
             }
             first=false;
         }
         return lb;

      },
      setAge : function (age) {
              this.childNodes.filter(function(s) {
                      return s.hasOwnProperty('uid')&&s.hasOwnProperty("gene");
                  }).forEach(function (s) {
                          s.recomputeAgeIndicator();
                      });
      },
      ///When the make new lobe button is pressed this item is invoked and makes a new lobe calls performedAction on it to populate the undos
      makeNewLobe : function () {
          return this.makeNewSelectable(new LobeOutput(this,new Gene(this,new Elysia.Genome.Gene())));
      },
      makeSelectedAppearAt : function (age) {
          var first=true;
          for (var uid in this.context.selection) {
              var redo=function (lobe) {
                  return function() {
                      lobe.setMinAge(age);
                  };
              }(this.context.selection[uid]);
              var undo=function (lobe) {
                  var oldAge=lobe.minAge;
                  return function() {
                      lobe.setMinAge(oldAge);
                  };
              }(this.context.selection[uid]);
              redo();
              this.context.performedAction(redo,undo);
              if (!first) {
                  this.context.coalesceUndos();
              }
              first=false;
          }          
      },
      loadGenome:function(genomeData,father){
          alert("Loading "+genomeData)
          var savemessage=new Elysia.Genome.Genome();
          if (savemessage.ParseFromStream(new PROTO.Base64Stream(genomeData))) {
              if (father) {
                  this.loadHaploid (savemessage.fathers);
              }
              if (father) {
                  this.loadHaploid (savemessage.mothers);
              }
              this.saveString=genomeData;
          }
      },
      saveAsGenome:function(father,mother){
          var savemessage=new Elysia.Genome.Genome();
          if (father==false||mother==false) {
              savemessage.ParseFromStream(new PROTO.Base64Stream(this.saveString));
          }
          var haploid=new Elysia.Genome.Chromosome();
          //FIXME: save entire editor state
          if (father) {
              savemessage.fathers=haploid;
          }
          if (mother) {
              savemessage.mothers=haploid;
          }
          var b64stream = new PROTO.Base64Stream();          
          savemessage.SerializeToStream(b64stream);
          alert(b64stream.getString().length);
      },
      makeSelectedVanishAt: function (age) {
          var first=true;
          for (var uid in this.context.selection) {
              var redo=function (lobe) {
                  return function() {
                      lobe.setMaxAge(age);
                  };
              }(this.context.selection[uid]);
              var undo=function (lobe) {
                  var oldAge=lobe.maxAge;
                  return function() {
                      lobe.setMaxAge(oldAge);
                  };
              }(this.context.selection[uid]);
              redo();
              this.context.performedAction(redo,undo);
              if (!first) {
                  this.context.coalesceUndos();
              }
              first=false;
          }          
      },
      ///When a new lobe is created and passed in, this attaches it to the scene graph and make the do and undo functions for it
      makeNewSelectable : function (lobe) {

         var thus = this;
         var isSelected=lobe.selected;
         var context=this.context;
         redoFunction=function(){thus.append(lobe);if (isSelected) {context.select(lobe);}};
         undoFunction=function(){if(lobe.isSelected()) {isSelected=true;}context.deselect(lobe);thus.remove(lobe);};
         redoFunction();
         this.context.performedAction(redoFunction,undoFunction);
         return lobe;
      },
      createLobesOnPredicate : function (predicate,lobeCreator) {
          var createdLobes={};
          this.childNodes.filter(function(s) {
              try {
                  if(s.hasOwnProperty('gene')) {
                      if (predicate(s)) {
                          return true;
                      }
                  }
              }catch(e) {return false;}
              return false;
          }).forEach(function(lobe) {
                  createdLobes[lobe.uid]=lobe.getBoundingBox();
              });
          for (var uid in createdLobes) {
              lobeCreator(createdLobes[uid]);
          }
      },
      lobeProperties: function() {
          if (!mapEmpty(this.context.selection)) {
              ifrm=new LobeDiv(this);
          }
      },
      undo : function () {
          this.context.undo();
      },
      redo : function () {
          this.context.redo();
      },
      beginEditing : function() {
          this.root.dispatchEvent({type: 'started', canvasTarget : this });
      },
      ///make some html that describes the current action appear ontop of the canvas
      showDescription : function() {
        var desc = E('div')
        desc.appendChild(E('h1', this.name));
        desc.appendChild(E('div', this.description));
        this.query(desc,
          'Begin editing', this.beginEditing,
          'Back to main menu', function() { this.parentNode.returnToMenu() }
        );
      },
      ///make some HTML appear ontop of the Canvase
      query : function(header) {
        var div = E('div', {className : 'message'});
        var msg = new ElementNode(div,
          { x : windowWidth/4, y : windowHeight/8, align : 'center' });
        var msgDiv = E('div', header);
        div.appendChild(msgDiv);
        var options = E('div');
        var th = this;
        for (var i=1; i<arguments.length; i+=2) {
          var name = arguments[i];
          var callback = arguments[i+1];
          options.appendChild(E('h2', name, {
            onclick : (function(callback){ return function() {
              if (!this.clicked) {
                callback.call(th);
                this.clicked = true;
                msg.after(500, msg.removeSelf);
                msg.animateTo('opacity', 0, 500, 'sine');
              }
            };})(callback),
            style: { cursor : 'pointer' }
          }));
        }
        div.appendChild(options);
        msg.opacity = 0;
        msg.animateTo('opacity', 1, 500, 'sine');
        this.messageLayer.append(msg);
      },
      ///Unknown
      notify : function(message, after, duration) {
        if (!after) after = 0
        this.after(after, function(){
          var msg = new ElementNode(E('h3', message),
            { x : windowWidth/4, y : windowHeight/20, align : 'center' });
          if (!duration) duration = 3500 + msg.element.textContent.length * 10;
          msg.opacity = 0;
          msg.animateTo('opacity', 1, 500, 'sine');
          msg.after(duration, function() {
            this.animateTo('opacity', 0, 500, 'sine');
          });
          msg.after(duration+500, msg.removeSelf);
          this.messageLayer.append(msg);
        });
      },

      ///Return to the menu that will eventually allow load/save of items
      returnToMenu : function() {
        if (this.completed) return;
        this.failed = true;
        this.after(1000, function() {
          this.query(E('h1', "Do you wish to return to menu?"),
            "Return to editing", function() { this.parentNode.tryAgain(); },
            "Back to main menu", function() { this.parentNode.returnToMenu(); }
          );
        });
      }



    });



    /// This editor function makes a new blank brain
    var NewBrain = Klass(Editor, {
      width : windowWidth,
      height : windowHeight,
      scale : 1,

      name : "Start new brain",
      description : "Make basic lobes that control reactions to food when hungry.",

      initialize : function(genomeEditor) {
        Editor.initialize.call(this,genomeEditor,"",true);
        this.when('started', function() {

        });
      }

    });

    ///This editor function gives a menu whether to make new, load, or get editor help
    var Menu = Klass(Editor, {
      width : windowWidth,
      height : windowHeight,
      scale : 1,

      initialize : function(genomeEditor) {
        Editor.initialize.call(this,genomeEditor,"",true);
        this.menu = new CanvasNode();
        this.menu.scale = 1;
        this.menu.zIndex = 1002;
        this.append(this.menu);
        this.setupMenu();
        this.selectRect.opacity = 0;
      },
      setAge: function (age) {},
      showDescription : function() {},
      makeNewLobe : function () {
                //noop
      },
      deleteLobe: function () {

      },
      makeSelectedAppearAt : function (age) {
                //noop
      },
      makeSelectedVanishAt: function (age) {

      },
      lobeProperties : function () {
                //noop
      },
      ///This is the fancy splash screen
      setupMenu : function() {
        var elem = E('h1');
        elem.appendChild(T('ELYSIA GENOME EDITOR'));

        var title = new ElementNode(elem, {
          x: windowWidth/4, y: windowHeight/16, zIndex: 1002, align: 'center', cursor: 'default'
        });
        var th = this;
        var controls = new CanvasNode();
        var bg = new ElementNode(E('div', {
          style: {
            width : windowWidth+'px',
            height : windowHeight+'px',
            backgroundColor: this.bgColor,
            opacity: 1.0
          }
        }), {zIndex : 1001});
        controls.append(bg);
        controls.display = 'none';
        controls.opacity = 0;
        var editorList = E('ol');
        GenomeEditor.editors.slice(1).forEach(function(lvl, i) {
          var li = E('li', E('h3', (i+1) + '. ' + lvl.prototype.name));
          li.onclick = function(){
            if (th.clicked) return;
            th.clicked = true;
            th.menu.controls.animateTo('opacity', 0, 300, 'sine');
            th.after(300, function() {
              this.parentNode.newEditorState(GenomeEditor.editors.indexOf(lvl));
            });
          };
          li.style.cursor = 'pointer';
          editorList.appendChild(li);
        });
        var editorHeader = E('h2', 'Actions');
        var jump = new ElementNode(editorHeader, {
          zIndex : 1002,
          x : windowWidth/4, y : windowHeight/8,
          align : 'center'
        });
        var editors = new ElementNode(editorList, {
          zIndex : 1002,
          x : windowWidth/4, y : windowHeight/4,
          align : 'center'
        });
        var divider = new Rectangle(windowWidth*.5, 1, {
          centered: true, x: windowWidth/2, y: 128, fill: 'red'
        });
        controls.append(jump, editors, divider);
        this.menu.title = title;
        this.menu.controls = controls;
        this.menu.append(title);
        this.menu.append(controls);
        this.bg.addEventListener('click', function() {
          if (!th.menuVisible) {
            th.showMenu();
          }
        }, false);
      },

      showMenu : function() {
        if (this.menuVisible) return;
        this.menuVisible = true;
        var th = this;
        this.menu.controls.display = 'block';
        this.menu.controls.animateTo('opacity', 1, 500, 'sine');
        this.menu.after(10000, function() {
          this.controls.animateTo('opacity', 0, 500, 'sine');
          this.after(500, function() {
            this.controls.display = 'none';
            th.menuVisible = false;
          });
        });
      }
    });


    ///This is the singleton object that represents the whole of the editor and the control dialog that is always visible (to make new lobes, etc)
    var GenomeEditor = Klass(CanvasNode, {
      editorIndex : 0,
      editors : [Menu, NewBrain],

      initialize : function(canvasElem, devMode) {
        CanvasNode.initialize.call(this);
        this.canvas = new Canvas(canvasElem);
        this.canvas.frameDuration = 30;
        this.canvas.append(this);
        this.x=windowWidth/2;
        this.y=windowHeight/2;
        this.canvas.fixedTimestep = true;
        this.canvas.clear = false;
        this.returnToMenu();
        this.setupEtc();
        if (devMode) {
            this.newEditorState(1);
        }
      },

      returnToMenu : function() {
        this.editorIndex = 0;
        this.changeEditor(this.editors[this.editorIndex]);
      },

      nextEditor : function() {
        this.editorIndex++;
        var editor = this.editors[this.editorIndex % this.editors.length];
        this.changeEditor(editor);
      },

      newEditorState : function(idx) {
        this.editorIndex = idx;
        var editor = this.editors[this.editorIndex % this.editors.length];
        this.changeEditor(editor);
      },

      tryAgain : function() {
        this.changeEditor(this.editors[this.editorIndex]);
      },
      currentEditor : function () {
         return this.editor;
      },
      changeEditor : function(editor) {
        if (this.editor) this.editor.removeSelf();
        if (editor) {
          this.editor = new editor(this);
          this.setAge(this.Age);
          this.append(this.editor);
        }
      },

      Radiation : true,
      setRadiation : function(fb) {
        this.Radiation = fb;
      },
      DrawAgeInactiveLobes : true,
      setDrawAgeInactiveLobes:function(doDraw){
         this.DrawAgeInactiveLobes=doDraw;
         this.setAge(this.Age);
      },
      Age : 1.0,
      setAge : function(s) {
        this.Age = parseFloat(""+s);
        this.editor.setAge(this.Age);        
      },
      makeSelectedAppearAtThisAge:function(s) {
         var age=parseFloat(""+this.Age);
         if (age>=.98) age=1.0;
         if (age<=.01) age=0.0;

         this.currentEditor().makeSelectedAppearAt(age);
      },
      makeSelectedVanishAtThisAge:function(s) {
         var age=parseFloat(""+this.Age);
         if (age>=.98) age=1.0;
         if (age<=.01) age=0.0;
         this.currentEditor().makeSelectedVanishAt(age);
      },
      makeNewLobe:function(s) {
         this.currentEditor().makeNewLobe();
      },
      deleteLobe:function(s) {
         this.currentEditor().deleteLobe();
      },
      lobeProperties:function(s) {
         this.currentEditor().lobeProperties();
      },
      undo:function(s) {
         this.currentEditor().undo();
      },
      redo:function(s) {
         this.currentEditor().redo();
      },
      saveAsFathersGenome:function() {
         this.currentEditor().saveAsGenome(true,false);
      },
      saveAsMothersGenome:function() {
         this.currentEditor().saveAsGenome(false,true);
      },
      saveAsBothSidesGenome:function() {
         this.currentEditor().saveAsGenome(true,true);
      },
      loadFromHttpUrl:function(url,father){
          var ed=this.currentEditor();
          var fileLoad=new XMLHttpRequest();
          fileLoad.onreadystatechange=function() {
              if (fileLoad.readyState == 4) {
                 
                  if (fileLoad.status==200||fileLoad.status==0)
                      ed.loadGenome(fileLoad.responseText,father);
                  else 
                      alert(url+" not found");                  
              }
          };
          fileLoad.overrideMimeType("text/plain");          
          fileLoad.open("GET",url,true);
          fileLoad.send();
      },
      loadMotherFromHttpUrl:function(url) {
          this.loadFromHttpUrl(url,false);
      },
      loadFatherFromHttpUrl:function(url) {
          this.loadFromHttpUrl(url,true);
      },
      ///This makes the global control panel that allows users to make new lobes, etc
      setupEtc : function() {
        this.canvas.updateFps = true;
        var debug = E('div');
        var t0 = -1;
        var frames = [];
        var fc = E.canvas(200, 10);
        var fpsE = T('');
        var elapsedE = T('');
        var realFpsE = T('');
        var elapsedRealE = T('');
        debug.append(fpsE, ' fps (', elapsedE, ' ms to draw scene)', E('br'),
          realFpsE, ' real fps (', elapsedRealE, ' ms between frames)',
          E('br'), fc);
        var fctx = fc.getContext('2d');
        fctx.globalCompositeOperation = 'copy';
        fctx.fillStyle = '#828292';
        this.canvas.addFrameListener(function(t) {
          if (this.updateFps) {
            fctx.drawImage(fc, -1, 0);
            fctx.clearRect(199, 0, 1, 10);
            fctx.fillRect(199, 0, 1, Math.min(100, this.currentRealFps) / 3.3);
            if (Math.floor(t / 500) != t0) {
              t0 = Math.floor(t / 500);
              var fps = (Math.floor(this.fps * 10)/10);
              var elapsed = Math.floor(1000 / this.fps);
              var realFps = (Math.floor(this.realFps * 10)/10);
              var elapsedReal = Math.floor(1000 / this.realFps);
              fpsE.textContent = fps;
              elapsedE.textContent = elapsed;
              realFpsE.textContent = realFps;
              elapsedRealE.textContent = elapsedReal;
            }
          }
        });
        this.canvasControlPanel = new GuiConfig({
          object : this.canvas,
          container : $('debug'),
          title : 'Debug',
          controls : [
            'updateFps',
            'playOnlyWhenFocused',
            'drawBoundingBoxes',
            ['useMockContext', 'boolean', {title: "Turn off drawing. Useful for benchmarking the AI."}]
          ]
        });
        this.canvasControlPanel.show();
        this.controlPanel = new GuiConfig({
          object : this,
          container : $('debug'),
          title : 'Lifespan',
          controls : [
            ['Age', '0.0..1.0'],
            'Radiation',
            'DrawAgeInactiveLobes',
            ['makeNewLobe','function'],
            ['deleteLobe','function'],
            ['lobeProperties','function'],
            ['makeSelectedAppearAtThisAge','function'],
            ['makeSelectedVanishAtThisAge','function'],
            ['redo','function'],
            ['undo','function'],
            ['saveAsFathersGenome','function'],
            ['saveAsMothersGenome','function'],
            ['saveAsBothSidesGenome','function'],
            ['loadMotherFromHttpUrl','string'],
            ['loadFatherFromHttpUrl','string']

          ]
        })
        this.controlPanel.show();
      }

    });


    ///Called when the page is onload()ed
    function init() {
      var c = E.canvas(windowWidth, windowHeight);
      var d = E('div', { id: 'screen' });
      d.appendChild(c);
      document.body.appendChild(d);
      var devmode=(window.location.search.split("devmode=true").length>1);
      var GE = new GenomeEditor(c,devmode);
    }
