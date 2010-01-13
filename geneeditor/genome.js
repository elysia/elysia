if (typeof(Elysia)=="undefined") {Elysia = {};}
if (typeof(Elysia.Genome)=="undefined") {Elysia.Genome = {};}
Elysia.Genome._PBJ_Internal="pbj-0.0.3";
Elysia.Genome.Protein = PROTO.Message("Elysia.Genome.Protein",{
	id: {
		options: {},
		multiplicity: PROTO.required,
		type: function(){return PROTO.uint64;},
		id: 1
	},
	Effect: PROTO.Enum("Elysia.Genome.Protein.Effect",{
		CUSTOM :0,
		GROW_LEAF :1,
		GROW_NEURON0 :2,
		GROW_NEURON1 :3,
		GROW_NEURON2 :4,
		GROW_NEURON3 :5,
		GROW_NEURON4 :6,
		GROW_NEURON5 :7,
		GROW_NEURON6 :8,
		GROW_NEURON7 :9,
	}),
	effect: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return Elysia.Genome.Protein.Effect;},
		id: 2
	},
});
Elysia.Genome.Proteins = PROTO.Message("Elysia.Genome.Proteins",{
	proteins: {
		options: {},
		multiplicity: PROTO.repeated,
		type: function(){return Elysia.Genome.Protein;},
		id: 1
	},
});
Elysia.Genome.Condition = PROTO.Message("Elysia.Genome.Condition",{
	protein: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return PROTO.uint64;},
		id: 1
	},
	Test: PROTO.Enum("Elysia.Genome.Condition.Test",{
		ANY_CONCENTRATION :0,
		CONCENTRATION_GREATER :1,
		CONCENTRATION_LESS :2,
		NO_CONCENTRATION :3,
	}),
	test: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return Elysia.Genome.Condition.Test;},
		id: 2
	},
	density: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return PROTO.float;},
		id: 3
	},
});
Elysia.Genome.ConditionClause = PROTO.Message("Elysia.Genome.ConditionClause",{
	disjunction: {
		options: {},
		multiplicity: PROTO.repeated,
		type: function(){return Elysia.Genome.Condition;},
		id: 1
	},
});
Elysia.Genome.Gene = PROTO.Message("Elysia.Genome.Gene",{
	position: {
		options: {},
		multiplicity: PROTO.required,
		type: function(){return PROTO.float;},
		id: 1
	},
	protein: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return PROTO.uint64;},
		id: 2
	},
	bounds: {
		options: {packed:true},
		multiplicity: PROTO.optional,
		type: function(){return PBJ.boundingbox3f3f;},
		id: 3
	},
	conjunction: {
		options: {},
		multiplicity: PROTO.repeated,
		type: function(){return Elysia.Genome.ConditionClause;},
		id: 4
	},
	target_region: {
		options: {packed:true},
		multiplicity: PROTO.optional,
		type: function(){return PBJ.boundingbox3f3f;},
		id: 5
	},
});
Elysia.Genome.Chromosome = PROTO.Message("Elysia.Genome.Chromosome",{
	genes: {
		options: {},
		multiplicity: PROTO.repeated,
		type: function(){return Elysia.Genome.Gene;},
		id: 1
	},
});
Elysia.Genome.Genome = PROTO.Message("Elysia.Genome.Genome",{
	fathers: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return Elysia.Genome.Chromosome;},
		id: 1
	},
	mothers: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return Elysia.Genome.Chromosome;},
		id: 2
	},
	proteins: {
		options: {},
		multiplicity: PROTO.optional,
		type: function(){return Elysia.Genome.Proteins;},
		id: 3
	},
});
