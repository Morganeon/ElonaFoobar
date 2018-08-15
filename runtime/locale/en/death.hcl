locale {
    death_by {
        chara {
            # _1 is victim, _2 is attacker.
            transformed_into_meat {
                active = "transform${s(_2)} ${him(_1)} into several pieces of meat."
                passive = "${name(_1)} ${is(_1)} transformed into several pieces of meat."
            }
            destroyed {
                active = "destroy${s(_2)} ${him(_1)}"
                passive = "${name(_1)} ${is(_1)} killed."
            }
            minced {
                active = "mince${s(_2)} ${him(_1)}."
                passive = "${name(_1)} ${is(_1)} minced."
            }
            killed {
                active = "kill${s(_2)} ${him(_1)}."
                passive = "${name(_1)} ${is(_1)} slain."
            }
            death_cause = "was killed by ${basename(_1)}"
        }
        element {
            # _1 is victim, _2 is attacker.
            default {
                active = "kill${s(_2)} ${him(_1)}."
                passive = "${name(_1)} ${is(_1)} killed."
            }
            _50 {
                active = "burn${s(_2)} ${him(_1)} to death."
                passive = "${name(_1)} ${is(_1)} burnt to ashes."
            }
            _51 {
                active = "transform${s(_2)} ${him(_1)} to an ice sculpture."
                passive = "${name(_1)} ${is(_1)} frozen and turn${s(_1)} into an ice sculpture."
            }
            _52 {
                active = "electrocute${s(_2)} ${him(_1)} to death."
                passive = "${name(_1)} ${is(_1)} struck by lightning and die${s(_1)}."
            }
            _53 {
                active = "let${s(_2)} the depths swallow ${him(_1)}."
                passive = "${name(_1)} ${is(_1)} consumed by darkness."
            }
            _54 {
                active = "completely disable${s(_2)} ${him(_1)}."
                passive = "${name(_1)} lose${s(_1)} ${his(_1)} mind and commit${s(_1)} a suicide."
            }
            _55 {
                active = "kill${s(_2)} ${him(_1)} with poison."
                passive = "${name(_1)} ${is(_1)} poisoned to death."
            }
            _56 {
                active = "entrap${s(_2)} ${him(_1)} into the inferno."
                passive = "${name(_1)} go${s(_1, true)} to hell."
            }
            _57 {
                active = "shatter${s(_2)} ${him(_1)} to atoms."
                passive = "${name(_1)} resonate${s(_1)} and break up."
            }
            _58 {
                active = "destroy${s(_2)} ${his(_1)} nerves."
                passive = "${name(_1)} die${s(_1)} from neurofibroma."
            }
            _59 {
                active = "let${s(_2)} the chaos consume ${him(_1)}."
                passive = "${name(_1)} ${is(_1)} drawn into a chaotic vortex."
            }
            # _60
            _61 {
                active = "cut${s(_2)} ${him(_1)} into thin strips."
                passive = "${name(_1)} ${is(_1)} cut into thin strips."
            }
            # _62
            _63 {
                active = "melt${s(_2)} ${him(_1)} away."
                passive = "${name(_1)} melt${s(_1)}."
            }
        }
        other {
            _1 {
                text = "${name(_1)} ${is(_1)} caught in a trap and die${s(_1)}."
                death_cause = "got caught in a trap and died"
            }
            _2 {
                text = "${name(_1)} die${s(_1)} from over-casting."
                death_cause = "was completely wiped by magic reaction"
            }
            _3 {
                text = "${name(_1)} ${is(_1)} starved to death."
                death_cause = "was starved to death"
            }
            _4 {
                text = "${name(_1)} ${is(_1)} killed with poison."
                death_cause = "miserably died from poison"
            }
            _5 {
                text = "${name(_1)} die${s(_1)} from curse."
                death_cause = "died from curse"
            }
            _6 {
                text = "${name(_1)} ${is(_1)} squashed by ${_2}."
                death_cause = "was squashed by ${_1}"
                backpack = "backpack"
            }
            _7 {
                text = "${name(_1)} tumble${s(_1)} from stairs and die${s(_1)}."
                death_cause = "tumbled from stairs and died"
            }
            _8 {
                text = "${name(_1)} ${is(_1)} killed by an audience."
                death_cause = "was killed by an audience"
            }
            _9 {
                text = "${name(_1)} ${is(_1)} burnt and turned into ash."
                death_cause = "was burnt and turned into ash"
            }
            # _10
            _11 {
                text = "${name(_1)} ${is(_1)} assassinated by the unseen hand."
                death_cause = "got assassinated by the unseen hand"
            }
            _12 {
                text = "${name(_1)} ${is(_1)} killed by food poisoning."
                death_cause = "got killed by food poisoning"
            }
            _13 {
                text = "${name(_1)} die${s(_1)} from loss of blood."
                death_cause = "died from loss of blood"
            }
            _14 {
                text = "${name(_1)} die${s(_1)} of the Ether disease."
                death_cause = "died of the Ether disease"
            }
            _15 {
                text = "${name(_1)} melt${s(_1)} down."
                death_cause = "melted down"
            }
            _16 {
                text = "${name(_1)} shatter${s(_1)}."
                death_cause = "committed suicide"
            }
            _17 {
                text = "${name(_1)} ${is(_1)} turned into atoms."
                death_cause = "was killed by an atomic bomb"
            }
            _18 {
                text = "${name(_1)} step${s(_1)} into an iron maiden and die${s(_1)}."
                death_cause = "stepped in an iron maiden and died"
            }
            _19 {
                text = "${name(_1)} ${is(_1)} guillotined and die${s(_1)}."
                death_cause = "was guillotined"
            }
            _20 {
                text = "${name(_1)} hang${s(_1)} ${his(_1)}self."
                death_cause = "commited suicide by hanging"
            }
            _21 {
                text = "${name(_1)} choke${s(_1)} on mochi and die."
                death_cause = "ate mochi and died"
            }
        }
    }

}
