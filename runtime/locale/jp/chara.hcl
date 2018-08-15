locale {
    chara {
        you = "あなた"
        something = "何か"

        age_unknown = "不明"

        name_with_title = "《${basename(_1)}》"

        contract_expired = "${basename(_1)}との契約期間が切れた。"

        corruption {
            symptom = "エーテルの病が発症した。"
            add = "あなたはエーテルに侵食された。"
            remove = "あなたのエーテルの侵食はやわらいだ。"
        }

        gain_level {
            self = "${name(_1)}はレベル${_2}になった！"
            other = "${name(_1)}は成長した。"
        }

        weight {
            gain = "${name(_1)}は太った。"
            lose = "${name(_1)}は痩せた。"
        }

        height {
            gain = "${name(_1)}の身長は少し伸びた。"
            lose = "${name(_1)}の身長は少し伸びた。"
        }

        place_failure {
            ally = "${name(_1)}とはぐれた。"
            other = "${name(_1)}は何かに潰されて息絶えた。"
        }

        impression {
            gain = "${basename(_1)}との関係が<${_2}>になった！"
            lose = "${basename(_1)}との関係が<${_2}>になった…"
        }

        job {
            general_vendor = "雑貨屋の${_1}"
            innkeeper = "宿屋の${_1}"
            trader = "交易店の${_1}"
            goods_vendor = "何でも屋の${_1}"
            baker = "パン屋の${_1}"
            magic_vendor = "魔法店の${_1}"
            blacksmith = "武具店の${_1}"
            trainer = "ギルドの${_1}"
            fisher = "釣具店の${_1}"
            blackmarket = "ブラックマーケットの${_1}"
            food_vendor = "食品店${_1}"
            souvenir_vendor = "おみやげ屋の${_1}"

            # Vernis
            of_vernis = "ヴェルニースの${_1}"

            # Palmia
            of_palmia = "パルミア市街地の${_1}"

            # Noyel
            street_vendor = "屋台商人の${_1}"
            street_vendor2 = "屋台商人屋の${_1}"
            of_noyel = "ノイエルの${_1}"
            fanatic {
                _0 = "オパートスの信者"
                _1 = "マニの信者"
                _2 = "エヘカトルの信者"
            }

            # Yowyn
            horse_master = "馬屋の${_1}"
            of_yowyn = "ヨウィンの${_1}"

            # Derphy
            slave_master = "謎の奴隷商人"
            of_derphy = "ダルフィ${_1}"

            # Port Kapul
            of_port_kapul = "ポート・カプールの${_1}"

            # Lumiest
            of_lumiest = "ルミエストの${_1}"

            # High Mountain
            dye_vendor = "染色店の${_1}"

            # Mages Guild
            spell_writer = "魔法書作家の${_1}"

            # Thieves Guild
            fence = "盗賊店の${_1}"

            # Encounter
            wandering_vendor = "行商人の${_1}"


            own_name = "${_1}の${_2}"

            alien {
                child = "の子供"
                alien_kid = "エイリアンの子供"
                child_of = "${_1}の子供"
            }

            shade = "シェイド"
        }
    }
}
